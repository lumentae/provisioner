#pragma once
#include <string>
#include <Globals.h>

namespace provisioner::project::mods
{
    class Mod
    {
    public:
        static void Add(std::string id, std::string version = "latest", bool noSearch = false, bool force = false);
        static void Remove(const std::string& id);
        static void Update(const std::string& id, bool force = false);
        static void Download(ModData& mod);
    };
}