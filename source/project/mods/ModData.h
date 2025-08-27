#pragma once

#include <utils/Json.h>

namespace provisioner::project::mods
{
    struct ModRequirement
    {
        std::string project_id = "";
        std::optional<std::string> version_id = "";
        std::string dependency_type = "";
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(ModRequirement, project_id, version_id, dependency_type)

    struct ModUpdate
    {
        std::string id;
        std::string version;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(ModUpdate, id, version)

    struct ModDownload
    {
        std::string url;
        int size;
        std::string sha1;
        std::string sha512;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(ModDownload, url, size, sha1, sha512)

    struct ModData
    {
        std::string name = "Unnamed";
        std::string id = "";
        std::string slug = "";
        std::string platform = "";
        ModDownload download = ModDownload{};
        ModUpdate update = ModUpdate{};
        std::vector<ModRequirement> requirements = {};
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(ModData, name, id, slug, platform, download, update, requirements)
};