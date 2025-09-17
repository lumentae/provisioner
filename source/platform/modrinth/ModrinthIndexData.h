#pragma once

#include <nlohmann/json.hpp>

#include "project/mods/ModData.h"

namespace provisioner::platform::modrinth
{
    struct ModrinthIndexFile
    {
        std::string path;
        std::map<std::string, std::string> hashes;
        std::map<std::string, std::string> env;
        std::vector<std::string> downloads;
        int fileSize{};
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(ModrinthIndexFile, path, hashes, env, downloads, fileSize)

    struct ModrinthIndexData
    {
        int formatVersion = 1;
        std::string game = "minecraft";
        std::string versionId;
        std::vector<ModrinthIndexFile> files;
        std::map<std::string, std::string> dependencies;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(ModrinthIndexData, formatVersion, game, versionId, files,
                                                    dependencies)

    inline ModrinthIndexFile GetIndexFileFromModData(const project::mods::ModData& modData)
    {
        ModrinthIndexFile indexFile;
        indexFile.path = "mods/" + modData.slug + ".jar";
        indexFile.fileSize = modData.download.size;
        indexFile.hashes = {
            {"sha1", modData.download.sha1},
            {"sha512", modData.download.sha512}
        };
        // TODO: Add this to moddata
        indexFile.env = {
            {"client", "required"},
            {"server", "required"}
        };
        indexFile.downloads = {
            modData.download.url
        };
        return indexFile;
    }
};
