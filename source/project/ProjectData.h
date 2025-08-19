#pragma once

#include <Common.h>

namespace provisioner::project
{
    struct MinecraftOptions
    {
        std::string version = "latest";
        std::string type = "vanilla";
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(MinecraftOptions, version, type)

    struct ProjectData
    {
        std::string name = "Unnamed";
        MinecraftOptions minecraft = MinecraftOptions{};
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(ProjectData, name, minecraft)
};
