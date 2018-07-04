#include "world_streamer.hpp"
#include <rapidxml.hpp>
#include "util/file_to_string.hpp"
#include "math/vec2i.hpp"
#include "entity_factory.hpp"
#include "dispatcher.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <typeinfo>
#include <regex>
#include <cmath>

using namespace std;
using namespace rapidxml;

// helper
/** \brief returns whether a given string is the name of a cell file
 * \param the name of the file
 * \return whether it is a cell file
 *
 */
bool isCellFile(string fileName)
{

    string minSample = "cell_0_0.xml";
    if( fileName.size() < minSample.size() ) return false;

    int slash = 0;
    for(int i = fileName.size(); i>=0; i--)
    if( fileName[i] == '/' )
    {
        slash = i;
        slash++;
        break;
    }
    int i = slash;

    string s1 = "cell_";

    for(unsigned int j=0; j<s1.size(); j++, i++)
    {
        if( s1[j] != fileName[i] ) return false;
    }

    if( fileName[i] == '0' )
    {

        if( fileName[i+1] != '_' ) return false;
        i += 2;

    }
    else
    {

        if( !('1' <= fileName[i] && fileName[i] <= '9') ) return false;
        i++;

        while( '0' <= fileName[i] && fileName[i] <= '9' )
        {
            i++;
        }

        if( fileName[i] != '_' ) return false;

        i++;

    }

    if( fileName[i] == '0' )
    {

        if( fileName[i+1] != '.' ) return false;
        i += 2;

    }
    else
    {

        if( !('1' <= fileName[i] && fileName[i] <= '9') ) return false;
        i++;

        while( '0' <= fileName[i] && fileName[i] <= '9' )
        {
            i++;
        }

        if( fileName[i] != '.' ) return false;

        i++;

    }

    string s2 = "xml";

    for(unsigned int j=0; j<s2.size(); j++, i++)
    {
        if( fileName[i] != s2[j] ) return false;
    }

    return true;

}

// helper
/** \brief returns the cell position from the file name
 * \param the name of the file
 * \return the position of the cell
 * For example "cell_3_4.xml" -> Vec2i(3, 4). <br>
 * Assumes the the file name is valid (i.e. has been
 * checked with isCellFile()
 */
Vec2i getCell(string fileName)
{

    int slash = 0;
    for(int i = fileName.size(); i>=0; i--)
    if( fileName[i] == '/' )
    {
        slash = i;
        slash++;
        break;
    }
    int i = slash;

    while( !('0' <= fileName[i] && fileName[i] <= '9')  )
    {
        i++;
    }

    int x = 0;
    while( '0' <= fileName[i] && fileName[i] <= '9' )
    {
        x *= 10;
        x += (int)(fileName[i] - '0');

        i++;
    }

    while( !('0' <= fileName[i] && fileName[i] <= '9')  )
    {
        i++;
    }

    int y = 0;
    while( '0' <= fileName[i] && fileName[i] <= '9' )
    {
        y *= 10;
        y += (int)(fileName[i] - '0');

        i++;
    }

    return Vec2i(x, y);

}




WorldStreamer::WorldStreamer
(
	string worldFolder,
	float cellSize,
    unsigned int windowSize
)
:
cellSize(cellSize),
worldWindow(windowSize)
{

	this->worldFolder = worldFolder;

    fstream fs;
    fs.open("world_folder/world_file.txt", ios::in);

    int x, y;
    while(fs >> x)
    {
        fs >> y;
        Vec2i cell(x, y);
        availableCells.insert( cell );
    }

}

void WorldStreamer::init(const Vec2i& cell, const Vec2f& offset)
{

    windowPos = cell;

    for
    (
        int y = cell.y - worldWindow.windowSize;
        y <= (int)(cell.y + worldWindow.windowSize);
        y++
    )
    for
    (
        int x = cell.x - worldWindow.windowSize;
        x <= (int)(cell.x + worldWindow.windowSize);
        x++
    )
    if( availableCells.find( Vec2i(x, y) ) != availableCells.end() )
    {
        loadCellAtPosition(Vec2i(x, y));
    }

}

void WorldStreamer::update(const Vec2f& position)
{

    // check if the entities have changed of cell

    for
    (
        auto it = worldWindow.cells.begin();
        it != worldWindow.cells.end();
        ++it
    )
    {

        WorldCell& wc = it->second->get_mutable();
        Vec2i currentCell = it->first;

        for(unsigned int i=0; i<wc.entities.size(); i++)
        {
            Entity& ent = wc.entities[i];

            Vec2f& pos = service::entity::ref().registry.get<Vec2f>(ent);
            Vec2i goodCell = Vec2i( floor(pos.x/cellSize), floor(pos.y/cellSize) );
            goodCell += windowPos;

            // the entity is not in the cell it should be
            // --> change of cell
            if( currentCell != goodCell )
            {

                auto it2 = worldWindow.cells.find( goodCell );
                // the cell is loaded -> just append
                if( it2 != worldWindow.cells.end() )
                {
                    wc.entities[i] = wc.entities.back();
                    wc.entities.pop_back();

                    it2->second->get_mutable().entities.push_back(ent);

                    i--;

                }

                // the cell is not loaded, what to do?
                // --> load it
                else if( availableCells.find(goodCell) != availableCells.end() )
                {
                    loadCellAtPosition(goodCell);
                }

            }

        }

    }

    // check if the main character changed of cell

    Vec2i nextCell = windowPos;
    Vec2f pos = position;

    while( pos.x < 0.0f )
    {
        nextCell.x--;
        pos.x += cellSize;
    }
    while( pos.x > cellSize )
    {
        nextCell.x++;
        pos.x -= cellSize;
    }

    while( pos.y < 0.0 )
    {
        nextCell.y--;
        pos.y += cellSize;
    }
    while( pos.y > cellSize )
    {
        nextCell.y++;
        pos.y -= cellSize;
    }

    // changed of cell
    // must change the coordinate system origin
    if( nextCell != windowPos )
    {

        // update window pos
        windowPos = nextCell;

        service::dispatcher::ref().trigger<Vec2f::RepositionEvent<Entity>>(position, pos);

        // delete old cells
        for
        (
            auto it = worldWindow.cells.begin();
            it != worldWindow.cells.end();
            // ++it
        )
        {

            if
            (
                (int)(nextCell.x - worldWindow.windowSize) > it->first.x ||
                it->first.x > (int)(nextCell.x + worldWindow.windowSize) ||
                (int)(nextCell.y - worldWindow.windowSize) > it->first.y ||
                it->first.y > (int)(nextCell.y + worldWindow.windowSize)
            )
            {

                // update XML document
                auto loaded_it = loadedCellResources.find(it->first);

                loadedCellResources.erase(loaded_it);

                service::resource::ref().release(it->second);
                auto nextIt = it;
                nextIt++;
                worldWindow.cells.erase(it);
                it = nextIt;



                continue;


            }

            it++;

        }

        // insert new cells
        for
        (
            int y = (int)(nextCell.y - worldWindow.windowSize);
            y <= (int)(nextCell.y + worldWindow.windowSize);
            y++
        )
        for
        (
            int x = (int)(nextCell.x - worldWindow.windowSize);
            x <= (int)(nextCell.x + worldWindow.windowSize);
            x++
        )
        if
        (
            worldWindow.cells.find( Vec2i(x, y) ) == worldWindow.cells.end() &&     // no
            availableCells.find( Vec2i(x, y) ) != availableCells.end() &&           // yes
            loadingCellResources.find( Vec2i(x, y) ) == loadingCellResources.end()  // no
        )
        {
            loadCellAtPosition(Vec2i(x, y));
        }


    }

    // parse loaded cell resources
    for (
        auto it = loadingCellResources.begin();
        it != loadingCellResources.end();
        //++it
    )
    {

        auto& res = const_cast<WorldCell::Resource&>(it->second.get());
        // the resource file is finally loaded
        if
        (
            res.get_status() == WorldCell::Status::LOADED // finally loaded
        )
        {

            /*
            // loaded late.
            // finally loaded but it is not needed anymore,
            // so release resource without parsing
            if
            (
                (int)(windowPos.x - worldWindow.windowSize) > it->first.x ||
                it->first.x > (int)(windowPos.x + worldWindow.windowSize )||
                (int)(windowPos.y - worldWindow.windowSize) > it->first.y ||
                it->first.y > (int)(windowPos.y + worldWindow.windowSize)
            )
            {

                ResourceManager* resMan = ResourceManager::getSingleton();
                resMan->releaseCell( res );
                loadingCellResources.erase( it );
                continue;

            }
            */

            Vec2i cell = it->first;
            worldWindow.cells[ cell ] = it->second;

            auto nextIt = it;
            nextIt++;
            loadedCellResources[ cell ] = it->second;
            loadingCellResources.erase( it );
            it = nextIt;


            continue;

        }
        it++;
    }

}

const Vec2i& WorldStreamer::getWindowPosition()const
{
	return windowPos;
}

const char* WorldStreamer::getWorldFolder() const
{
    return worldFolder.c_str();
}

float WorldStreamer::getCellSize()const
{
    return cellSize;
}

vector<Entity> WorldStreamer::getEntities()const
{
    return worldWindow.getEntities();
}

void WorldStreamer::end()
{

    for
    (
        auto it = worldWindow.cells.begin();
        it != worldWindow.cells.end();
        // ++it
    )
    {
        auto loaded_it = loadedCellResources.find(it->first);

        loadedCellResources.erase(loaded_it);

        service::resource::ref().release(it->second);

        auto nextIt = it;
        nextIt++;
        worldWindow.cells.erase(it);
        it = nextIt;

    }


}

void WorldStreamer::loadCellAtPosition(const Vec2i& position)
{

    stringstream ss;
    ss << worldFolder
        << "/"
        << "cell_"
        << position.x
        << "_"
        << position.y
        << ".xml";

    string fileName = ss.str();

    auto res = service::resource::ref().obtain<WorldCell::Resource>(core::resource::ID{ fileName.c_str() });

    res->get_mutable().position = position;

    loadingCellResources[position] = res;


}