#pragma once

#include "static_string.hpp"

namespace mfl
{
// TODO: An ugly macro for now, will change it later to compile-time convertion
#define value(x) static_string(#x)
}
