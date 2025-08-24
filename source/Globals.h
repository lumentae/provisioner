#pragma once
#include <memory>

#include "platform/IPlatform.h"

namespace provisioner::globals
{
    extern std::shared_ptr<platform::IPlatform> Platform;
}