#ifndef RESOURCE_MANAGER_TEMPLATES
#define RESOURCE_MANAGER_TEMPLATES

template<typename Type, typename... Args>
ResourceManager& ResourceManager::deliver(Args&&... args)
{
    m_registry.deliver<Type>(&m_shared_state, std::forward<Args>(args)...);

    return *this;
}

template<typename Type>
core::resource::Handle<Type> ResourceManager::obtain(const core::resource::ID& id)
{
    m_shared_state.m_mutex.lock();
    auto handle = m_registry.obtain<Type>(id);
    m_shared_state.m_mutex.unlock();

    return handle;
}

template<typename Type>
void ResourceManager::release(core::resource::Handle<Type>& handle)
{
    m_shared_state.m_mutex.lock();
    m_registry.release<Type>(handle);
    m_shared_state.m_mutex.unlock();
}



#endif // RESOURCE_MANAGER_TEMPLATES
