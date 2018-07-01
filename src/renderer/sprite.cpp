#include "sprite.hpp"
#include "graphics_system.hpp"
#include "../resource_manager/resource_manager.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <rapidxml.hpp>
#include "../util/exception.hpp"

using namespace std;
using namespace rapidxml;

// constants
const std::string Sprite::spritesPath = "sprites";


unsigned Sprite::getAnimIndex(const string& animName)const
{
    map<string, unsigned>::const_iterator it = animNameToIndex.find(animName);
    if(it == animNameToIndex.end()){
        cout << "warning: animation " << animName << " does not exist" << endl;
        return 0;
    }
    return it->second;
}

bool core::resource::LoaderProxy<Sprite::Resource>::load_synchronously(Sprite::Resource* ptr)
{
    ptr->m_status = Sprite::Status::START;

    return false;
}
bool core::resource::LoaderProxy<Sprite::Resource>::unload_synchronously(Sprite::Resource* ptr)
{
    for (auto& it : ptr->m_final.textures)
    {
        service::resource::ref().release(it);
    }

    return true;
}
bool core::resource::LoaderProxy<Sprite::Resource>::load_asynchronously(Sprite::Resource* ptr)
{
    ptr->m_status = Sprite::Status::LOADING_XML;
    
    auto path = Sprite::spritesPath + string("/") + ptr->m_id.c_str() + string(".xml");

    ptr->m_intermediate.load(path.c_str(), "sprite");
    
    auto root = ptr->m_intermediate.getNode();

    if (0 == root) throw Exception("expected <sprite> node");

    ptr->m_status = Sprite::Status::LOADED_XML;


    return true;
}
bool core::resource::LoaderProxy<Sprite::Resource>::unload_asynchronously(Sprite::Resource* ptr)
{
    return true;
}
bool core::resource::LoaderProxy<Sprite::Resource>::synchronize_loaded(Sprite::Resource* ptr)
{
    switch (ptr->m_status)
    {
    case Sprite::Status::LOADED_XML: {

        auto root = ptr->m_intermediate.getNode();
        // parse textures
        xml_node<>* nodeTextures = root->first_node("textures");
        if (0 == nodeTextures) throw Exception("expected <textures> node");

        xml_node<>* nodeTex = nodeTextures->first_node("tex");
        if (0 == nodeTex) throw Exception("expected <tex> node");

        map<string, string> idToName;
        do
        {
            xml_attribute<>* attrId = nodeTex->first_attribute("id");
            string id = string(attrId->value());
            if (0 == attrId) throw Exception("expected \"id\" attribute in <tex>");
            string name = string(nodeTex->value());
            map<string, string>::iterator it = idToName.find(id);
            if (it != idToName.end()) throw Exception("there is already a \"tex\" with id=" + id);
            idToName[id] = name;
            nodeTex = nodeTex->next_sibling("tex");
        } while (0 != nodeTex);

        ptr->m_final.textures.reserve(idToName.size());

        unsigned i = 0;
        for
            (
                map<string, string>::iterator it = idToName.begin();
                it != idToName.end();
                ++it
                )
        {

            ptr->m_final.textures.emplace_back(service::resource::ref().obtain<Texture::Resource>(core::resource::ID{ it->second.c_str() }));
            ptr->m_final.idToIndex[it->first] = i;
            ptr->m_final.textureNameToIndex[it->second] = i;
            i++;
        }

        ptr->m_status = Sprite::Status::LOADING_TEXTURES;
    } break;

    case Sprite::Status::LOADING_TEXTURES: {

        ptr->m_status = Sprite::Status::LOADED_TEXTURES;

        for (auto& texture : ptr->m_final.textures)
        {
            if (texture->get_status() != Texture::Status::READY)
            {
                ptr->m_status = Sprite::Status::LOADING_TEXTURES;
                break;
            }
        }

    } break;

    case Sprite::Status::LOADED_TEXTURES: {

        auto root = ptr->m_intermediate.getNode();
        // we are parsing the animations after the textures are loaded because we
        // need to know the dimensions of the textures
        xml_node<>* nodeAnimations = root->first_node("animations");
        if (0 == nodeAnimations) throw Exception("expected animations node");

        unsigned animIndex = 0;
        xml_node<>* nodeAnim = nodeAnimations->first_node("anim");
        if (0 == nodeAnim) throw Exception("you must provide at least one anim node");

        do
        {

            xml_attribute<>* attribAnimName = nodeAnim->first_attribute("id");
            if (0 == attribAnimName) throw Exception("no id provided for anim");
            string animName = string(attribAnimName->value());

            xml_node<>* nodeFrame = nodeAnim->first_node("frame");
            if (0 == nodeFrame)
            {
                throw Exception("you must provide at least one frame per anim");
            }

            Animation anim;
            anim.name = animName;

            do
            {

                xml_node<>* nodeTex = nodeFrame->first_node("tex");
                if (0 == nodeTex) throw Exception("no tex provided for frame");
                string texId = nodeTex->value();
                unsigned texIndex = ptr->m_final.idToIndex[texId];
                auto& tex = ptr->m_final.textures[texIndex]->get();

                xml_node<>* nodeRect = nodeFrame->first_node("rect");
                if (0 == nodeRect) throw Exception("no rect provided for frame");
                AARect rect;
                xml_node<>* nodeX = nodeRect->first_node("x");
                if (0 == nodeX) throw Exception("no x provided for rect");
                rect.x = atof(nodeX->value()) / tex.getWidth();

                xml_node<>* nodeY = nodeRect->first_node("y");
                if (0 == nodeY) throw Exception("no y provided for rect");
                rect.y = atof(nodeY->value()) / tex.getHeight();

                xml_node<>* nodeW = nodeRect->first_node("w");
                if (0 == nodeW) throw Exception("no w provided for rect");
                rect.w = atof(nodeW->value()) / tex.getWidth();

                xml_node<>* nodeH = nodeRect->first_node("h");
                if (0 == nodeH) throw Exception("no h provided for rect");
                rect.h = atof(nodeH->value()) / tex.getHeight();


                xml_node<>* nodeTime = nodeFrame->first_node("time");
                if (0 == nodeTime) throw Exception("no time provided for this frame");
                float time = atof(nodeTime->value());

                AnimationFrame frame;
                frame.textureIndex = texIndex;
                frame.vbo = LowLevelRenderer2D::SpriteVbo(rect);
                frame.frameDuration = time;
                anim.frames.push_back(frame);

                nodeFrame = nodeFrame->next_sibling("frame");

            } while (0 != nodeFrame);

            /*
            if(name == "grass")
            {
            cout << xmlText << endl;
            cout << anim.frames.size() << endl;
            }
            */

            ptr->m_final.animations.push_back(anim);
            ptr->m_final.animNameToIndex[animName] = animIndex;
            animIndex++;
            nodeAnim = nodeAnim->next_sibling("anim");

        } while (0 != nodeAnim);

        ptr->m_intermediate.free();

        ptr->m_status = Sprite::Status::EVERYTHING_LOADED;
    } break;

    default:
        break;
    }

    return ptr->m_status == Sprite::Status::EVERYTHING_LOADED;
}


