#pragma once

#include "static_string.hpp"
#include "variable.hpp"

namespace mfl
{
    template <static_string name, struct_field... fields>
    struct structure
    {
    };
}

