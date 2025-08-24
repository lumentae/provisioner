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
        if (std::filesystem::exists(modPath / (modData.slug + ".pm")))
        {
            spdlog::warn("Mod {} ({}) already exists", modData.name, modData.id);
            return;
        }

        spdlog::info("Adding mod {} ({})", modData.name, modData.id);

        for (const auto& requirement : modData.requirements)
        {
            AddMod(requirement.project_id);
        }

        const nlohmann::json json = modData;
        utils::WriteFile(modPath / (modData.slug + ".pm"), json.dump(4));
    }
}