#include "Globals.h"

#include "platform/modrinth/Modrinth.h"

namespace provisioner::globals
{
    std::shared_ptr<platform::IPlatform> Platform = std::make_shared<platform::modrinth::Modrinth>();
    // TODO: Support curseforge
}