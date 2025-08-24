#include "Mod.h"

#include <spdlog/spdlog.h>

#include "utils/File.h"

namespace provisioner::project::mods
{
    void Mod::AddMod(const std::string& id, std::string version = "latest")
    {
        const std::filesystem::path modPath = "mods";
        if (!std::filesystem::exists(modPath))
            std::filesystem::create_directory(modPath);

        if (version == "latest")
            version = globals::Platform->GetLatestVersion(id);

        auto modData = globals::Platform->GetModData(id, version);
        spdlog::info("Adding mod {} ({})", modData.name, version);

        const nlohmann::json json = modData;
        utils::WriteFile(modPath / (modData.name + ".pm"), json.dump(4));
    }
}