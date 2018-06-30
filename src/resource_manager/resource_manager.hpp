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
        virtual bool synchronize() = 0;
    };
    template<typename Type>
    struct LoaderProxy;

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
        Load,
        Unload,
        Stop
    };

    struct Request
    {
        ERequest m_type;
        TypeID m_type_id;
        ID m_id;
    };

    class SharedState
    {
    public:
        inline void lock() 
        {
            m_mutex.lock();
        }

        inline void unlock()
        {
            m_mutex.unlock();
        }

        template<typename Fn>
        inline void try_lock(Fn&& fn)
        {
            if (m_mutex.try_lock())
            {
                fn();

                m_mutex.unlock();
            }
            else
            {
                fn();
            }
        }

        inline void push_request(const Request& request) 
        { 
            m_request_queue.push(request); 
        }
        inline Request pop_request()
        {
            return m_request_queue.pop();
        }

        inline void stop()
        { 
            m_request_queue.push({ ERequest::Stop, 0, {""} }); 
        }
    private:
        std::mutex          m_mutex;
        WorkQueue<Request>  m_request_queue;
    };

    template<typename Intermediate, typename Final, typename Status = int>
    class Data
    {
        friend class  Loader<Data>;
        friend struct LoaderProxy<Data>;

        Data(const char* id) :
            m_id(id),
            m_status(Status{})
        { }
    public:
        using status_t = Status;

        Data() = delete;
        Data(const Data&) = delete;
        Data(Data&&) = default;

        operator const char *() const { return m_id.c_str(); }

        inline const Final & get() const { return m_final; }
        inline operator const Final &() const { return get(); }
        inline const Final & operator *() const { return get(); }
        inline const Final * operator ->() const { }

        inline ID               get_id() const { return ID{m_id.c_str()}; }
        inline Final &          get_mutable() const { return const_cast<Final&>(m_final); }
        inline const Status&    get_status() const { return m_status; }
    private:
        std::string     m_id;
        Status          m_status;
        Intermediate    m_intermediate;
        Final           m_final;
    };

    template<typename Type>
    struct LoaderProxy
    {
        static bool load_synchronously(Type*);
        static bool unload_synchronously(Type*);

        static bool load_asynchronously(Type*);
        static bool unload_asynchronously(Type*);

        static bool synchronize_loaded(Type*);
    };

    template<typename Type> 
    class Loader : public Registry::LoaderBasis<Type>
    {
    private:
        using Queue = std::deque<Type*>;
        using Proxy = LoaderProxy<Type>;
    public:
        Loader(SharedState* shared_state) :
            m_shared_state(shared_state)
        { }
        Loader(Loader&&) = default;
        Loader(const Loader&) = delete;
    public:
        std::shared_ptr<Type> load(const ID& id)
        {
            auto* ptr = new Type((const char*)id);

            if (!Proxy::load_synchronously(ptr))
            {
                m_shared_state->push_request({ ERequest::Load,GetTypeID<Type>(), id });
            }

            return std::shared_ptr<Type>(ptr);
        }
        bool unload(const Type& ref)
        {
            auto res = Proxy::unload_synchronously(const_cast<Type*>(&ref));

            if (!res)
            {
                m_shared_state->push_request({ ERequest::Unload,GetTypeID<Type>(), identify(ref) });
            }

            return res;
        }

        ID identify(const Type& ref)
        {
            return ID{ (const char*)ref };
        }

        void fulfill(const Request& request)
        {
            m_shared_state->lock();
            auto handle = cache->handle(request.m_id);
            auto ptr = const_cast<Type*>(&handle.get());
            m_shared_state->unlock();

            switch (request.m_type)
            {

            case ERequest::Load: {
                if (Proxy::load_asynchronously(ptr))
                {
                    m_shared_state->lock();
                    m_loaded_queue.emplace_back(ptr);
                    m_shared_state->unlock();
                }
            } break;

            case ERequest::Unload: {
                if (Proxy::unload_asynchronously(ptr))
                {
                    m_shared_state->lock();
                    cache->discard(request.m_id);
                    m_shared_state->unlock();
                }
            } break;

            default:
                break;
            }
        }
        bool synchronize()
        {
            if (m_loaded_queue.empty())
            {
                return false;
            }

            Queue pending;

            for (auto& ptr : m_loaded_queue)
            {
                if (!Proxy::synchronize_loaded(ptr))
                {
                    pending.emplace_back(ptr);
                }
            }

            m_loaded_queue = pending;

            return true;
        }
    private:
        SharedState*    m_shared_state;
        Queue           m_loaded_queue;
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

    template<typename Type, typename... Args>
    ResourceManager& deliver(Args&&... args);

    template<typename Type>
    core::resource::Handle<Type> obtain(const core::resource::ID& id);

    template<typename Type>
    void release(core::resource::Handle<Type>& handle);

    bool synchronize();
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
