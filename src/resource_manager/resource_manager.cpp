#include "resource_manager.hpp"
#include <iostream>

using namespace std;

void ResourceManager::launch()
{
    _stop = false;
    try
    {
        myThread = std::thread(ResourceManager::staticLoop, this);
    }
    catch (std::exception e)
	{
		cerr << "Error creating thread for ResouceManager: "
		<< e.what() << endl;
		exit(1);
	}
}

void ResourceManager::stop()
{
    m_shared_state.stop();
}

void ResourceManager::loop()
{

	// this might seem polling but it is not.
	// The pop function of WorkQueue will stall
	// if the are not elements in the queue
	while( !_stop )
	{
        auto request = m_shared_state.m_request_queue.pop();

        if (request.m_type == core::resource::ERequest::Stop)
        {
            _stop = true;
        }
        else
        {
            m_registry.loaders()[request.m_type_id]->fulfill(request);
        }


        std::this_thread::sleep_for(1ms);
	}

}
void* ResourceManager::staticLoop(void* object)
{

	((ResourceManager*)object)->loop();
	return 0;

}
