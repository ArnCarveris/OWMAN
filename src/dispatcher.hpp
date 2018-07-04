#pragma once

#include <entt/locator/locator.hpp>
#include <entt/signal/dispatcher.hpp>

namespace service
{
    using dispatcher = entt::ServiceLocator<entt::Dispatcher>;
}