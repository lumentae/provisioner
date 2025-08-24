#pragma once

#include <nlohmann/json.hpp>

namespace provisioner::project::mods
{
    struct ModUpdate
    {
        std::string platform;
        std::string id;
        std::string version;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(ModUpdate, platform, id, version)

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
        std::string fileName = "Unnamed";
        ModDownload download = ModDownload{};
        ModUpdate update = ModUpdate{};
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(ModData, name, fileName, download, update)
};