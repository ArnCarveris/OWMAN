#ifndef RESOURCE_MANAGER
#define RESOURCE_MANAGER

#include "work_queue.hpp"
#include <thread>
#include <mutex>
#include <string>
#include <deque>
#include <entt/locator/locator.hpp>
#include <entt/resource/registry.hpp>

namespace core::resource
{
    struct Request;

    template<typename> class Loader;struct ILoader
    {
        virtual void fulfill(const Request&) = 0;
    };

    using Registry = entt::ResourceRegistry<Loader, ILoader>;

    using ID = Registry::id_t;
    using TypeID = Registry::type_id_t;
    
    template<typename Type>
    using Cache = entt::ResourceCache<Type>;

    template<typename Type>
    using Handle = entt::ResourceHandle<Type>;


    template<typename Type>
    inline TypeID GetTypeID()
    {
        return Registry::type<Type>();
    }

    enum ERequest
    {
        Obtain,
        Release,
        Stop
    };

    struct Request
    {
        ERequest m_type;
        TypeID m_type_id;
        ID m_id;
    };

    struct SharedState
    {
        std::mutex         m_mutex;
        WorkQueue<Request> m_request_queue;

        void stop() { m_request_queue.push({ ERequest::Stop, 0, {""} }); }
    };

    template<typename Type> 
    class Loader : public Registry::LoaderBasis<Type>
    {
    public:
        Loader(SharedState* shared_state) :
            m_shared_state(shared_state)
        { }
        Loader(Loader&&) = default;
        Loader(const Loader&) = delete;
    public:
        std::shared_ptr<Type> load(const ID& id)
        {
            m_shared_state->m_request_queue.push({ ERequest::Obtain,GetTypeID<Type>(), id });

            return std::shared_ptr<Type>(new Type((const char*)id));
        }
        bool unload(const Type& ref)
        {
            m_shared_state->m_request_queue.push({ ERequest::Release,GetTypeID<Type>(), identify(ref)});

            return false;
        }

        ID identify(const Type& ref)
        {
            return ID{ ref.getName().c_str()};
        }

        void fulfill(const Request& request)
        {
            m_shared_state->m_mutex.lock();
            auto handle = cache->handle(request.m_id);
            auto& ref = const_cast<Type&>(handle.get());
            m_shared_state->m_mutex.unlock();

            switch (request.m_type)
            {

            case ERequest::Obtain: {
                ref.load();
            } break;

            case ERequest::Release: {
                ref.free();

                m_shared_state->m_mutex.lock();
                cache->discard(request.m_id);
                m_shared_state->m_mutex.unlock();
            } break;

            default:
                break;
            }
        }
    private:
        SharedState* m_shared_state;
    };
}


/** \brief This is the singleton you should use
 * Requesting resources will deliver a resource pointer
 * event if the resource data is not loaded.
 * You can check if a resource has been loaded checking
 * the status attribute.
 */
class ResourceManager
{

	std::thread myThread;
    
    core::resource::Registry    m_registry;
    core::resource::SharedState m_shared_state;
	
    bool _stop;

public:

	ResourceManager(){}

    template<typename Type>
    void deliver();

    template<typename Type>
    core::resource::Handle<Type> obtain(const core::resource::ID& id);

    template<typename Type>
    void release(core::resource::Handle<Type>& handle);

    /**
    * \brief launches the resource manger in its own thread
    */
    void launch();
	/**
	 * \brief stop the resource manager thread
	 */
	void stop();

private:

	/**
	 * \brief consumes requests
	 * Iterates for ever to handle requests.
	 */
	void loop();

	/**
	 * \brief this function calls loop
	 * You can not create a pthread from a member funtion
	 * so I use this static function as a helper
	 */
	static void* staticLoop(void* object);

};

namespace service
{
    using resource = entt::ServiceLocator<ResourceManager>;
}

#include "resource_manager_templates.hpp"

#endif
