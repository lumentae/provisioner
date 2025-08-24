#include "Globals.h"

#include "platform/Modrinth.h"

namespace provisioner::globals
{
    std::shared_ptr<platform::IPlatform> Platform = std::make_shared<platform::Modrinth>(); // TODO: Support curseforge
}