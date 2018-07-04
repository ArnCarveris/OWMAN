#include "world_window.hpp"
#include "world_cell.hpp"

using namespace std;

WorldWindow::WorldWindow()
{

}

WorldWindow::WorldWindow(unsigned int windowSize)
{

	this->windowSize = windowSize;

}

vector<Entity> WorldWindow::getEntities()const
{
    int len = 0;

    for (auto & wc : cells)
    {
        len += wc.second->get().entities.size();
    }

    vector<Entity> ents;
    ents.reserve(len);

    for (auto & wc : cells)
    {
        for (auto& e : wc.second->get().entities)
        {
            ents.push_back(e);
        }
    }

    return ents;

}

WorldWindow::~WorldWindow()
{

}
