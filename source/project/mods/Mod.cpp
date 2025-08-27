#include "Mod.h"

#include <spdlog/spdlog.h>

#include "project/Project.h"
#include "utils/File.h"

namespace provisioner::project::mods
{
    void Mod::Add(std::string id, std::string version = "latest")
    {
        REQUIRE_PROJECT();

        const std::filesystem::path modPath = "mods";
        if (!std::filesystem::exists(modPath))
            std::filesystem::create_directory(modPath);

        if (version == "latest")
            version = globals::Platform->GetLatestVersion(id);

        if (version.empty())
        {
            id = globals::Platform->Search(id);
            version = globals::Platform->GetLatestVersion(id);
        }

        auto modData = globals::Platform->GetModData(id, version);
        if (std::filesystem::exists(modPath / (modData.slug + ".pm")))
        {
            spdlog::info("Mod {} ({}) already exists", modData.name, modData.id);
            return;
        }

        spdlog::info("Adding mod {} ({})", modData.name, modData.id);

        for (const auto& requirement : modData.requirements)
        {
            if (requirement.dependency_type == "optional")
                continue;

            Add(requirement.project_id);
        }

        const nlohmann::json json = modData;
        utils::WriteFile(modPath / (modData.slug + ".pm"), json.dump(4));
    }

    void Mod::Remove(const std::string& id)
    {
        REQUIRE_PROJECT();

        const std::filesystem::path modPath = "mods";
        if (!std::filesystem::exists(modPath))
            std::filesystem::create_directory(modPath);

        const auto modFile = modPath / (id + ".pm");
        if (!std::filesystem::exists(modFile))
        {
            spdlog::warn("Mod {} does not exist", id);
            return;
        }

        std::filesystem::remove(modFile);
    }

    void Mod::Update(const std::string& id)
    {
    }

    void Mod::Download(const ModData& mod)
    {
        REQUIRE_PROJECT();

        const std::filesystem::path cachePath = ".cache";
        const std::filesystem::path modsPath = cachePath / "mods";
        if (!std::filesystem::exists(cachePath))
            std::filesystem::create_directory(cachePath);

        if (std::filesystem::exists(cachePath / (mod.slug + ".jar")))
        {
            spdlog::info("Mod {} already downloaded", mod.name);
            return;
        }

        spdlog::info("Downloading mod {}", mod.name);
        utils::DownloadFile(mod.download.url, modsPath / (mod.slug + ".jar"));
    }
}