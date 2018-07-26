#include "engine.hpp"

int main(int argc, char* argv[])
{
    service::engine::set("init_file.xml", "world_folder");
    service::engine::ref().init();
    service::engine::ref().mainLoop();
    service::engine::reset();

    return 0;
}
