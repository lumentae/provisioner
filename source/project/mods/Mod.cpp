#include "Mod.h"

#include <spdlog/spdlog.h>

#include "project/Project.h"
#include "utils/File.h"

namespace provisioner::project::mods
{
    void Mod::Add(std::string id, std::string version, const bool noSearch)
    {
        REQUIRE_PROJECT();

        if (project.mData.minecraft.type == "vanilla")
        {
            spdlog::error("Cannot add mods to a vanilla project");
            return;
        }

        const std::filesystem::path modPath = "mods";
        if (!std::filesystem::exists(modPath))
            std::filesystem::create_directory(modPath);

        if (!noSearch)
            id = globals::Platform->Search(id);

        if (version == "latest")
            version = globals::Platform->GetLatestVersion(id);

        auto modData = globals::Platform->GetModData(id, version);
        if (std::filesystem::exists(modPath / (modData.slug + ".pm")))
        {
            spdlog::info("Mod {} ({}) already exists", modData.name, modData.id);
            return;
        }

        spdlog::info("Adding mod {} ({})", modData.name, modData.id);

        for (const auto& [project_id, version_id, dependency_type] : modData.requirements)
        {
            if (dependency_type == "optional" || dependency_type == "incompatible")
                continue;

            const auto dependencyVersion = version_id.has_value() ? version_id.value() : "latest";
            Add(project_id, dependencyVersion, true);
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
        const std::filesystem::path modPath = "mods";
        if (!std::filesystem::exists(modPath))
            std::filesystem::create_directory(modPath);

        const auto modFile = modPath / (id + ".pm");
        if (!std::filesystem::exists(modFile))
        {
            spdlog::warn("Mod {} does not exist", id);
            return;
        }

        const std::string version = globals::Platform->GetLatestVersion(id);
        const ModData modData = nlohmann::json::parse(utils::ReadFile(modFile));

        spdlog::debug("Latest version for mod {} is {} (installed {})", id, version, modData.update.version);
        if (modData.update.version == version)
            return;

        std::filesystem::remove(modFile);
        Add(id, version, true);
    }

    void Mod::Download(const ModData& mod)
    {
        REQUIRE_PROJECT();

        const std::filesystem::path cachePath = ".cache";
        if (!std::filesystem::exists(cachePath))
            std::filesystem::create_directory(cachePath);

        const std::filesystem::path modPath = cachePath / (mod.slug + ".jar");

        if (std::filesystem::exists(modPath))
        {
            spdlog::info("Mod {} already downloaded", mod.name);
            return;
        }

        spdlog::info("Downloading mod {}", mod.name);
        utils::DownloadFile(mod.download.url, modPath);
    }
}