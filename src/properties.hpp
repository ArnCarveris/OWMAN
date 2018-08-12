#pragma once

#include "util/xml.hpp"

#include "entity.hpp"

namespace core
{
    using property = core::serialization::EntityPropertiesMediator<cereal::XMLRootInputArchive, cereal::XMLRootOutputArchive>;
}

