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
    core::resource::Handle<Type> handle;

    m_shared_state.try_lock([&]() { handle = m_registry.obtain<Type>(id); });

    return handle;
}

template<typename Type>
void ResourceManager::release(core::resource::Handle<Type>& handle)
{
    m_shared_state.try_lock([&]() { m_registry.release<Type>(handle); });
}



#endif // RESOURCE_MANAGER_TEMPLATES
