#pragma once

#include <Common.h>

namespace provisioner::project
{
    struct MinecraftOptions
    {
        std::string version = "latest";
        std::string type = "vanilla";
        std::string loaderVersion = "latest";
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(MinecraftOptions, version, type, loaderVersion)

    struct ProjectData
    {
        std::string name = "Unnamed";
        MinecraftOptions minecraft = MinecraftOptions{};
        std::vector<std::string> includes = {};
        std::vector<std::string> allowedVersions = {};
        /* ignored */
        std::pair<std::string, std::string> allowedVersionsString = std::make_pair("", "");
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(ProjectData, name, minecraft, includes, allowedVersions)
};