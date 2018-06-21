#ifndef RESOURCE_MANAGER_TEMPLATES
#define RESOURCE_MANAGER_TEMPLATES

template<typename Type>
void ResourceManager::deliver()
{
    m_registry.deliver<Type>(&m_shared_state);
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
