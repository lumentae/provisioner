#pragma once
#include <string>
#include <Globals.h>

namespace provisioner::project::mods
{
    class Mod
    {
    public:
        static void AddMod(const std::string& id, std::string version);
    };
}