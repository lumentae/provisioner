#include "Mod.h"

#include <sha1.h>
#include <sha3.h>
#include <spdlog/spdlog.h>

#include "project/Project.h"
#include "utils/File.h"
#include "utils/Prompt.h"

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

        std::string name = id;
        if (globals::Platform->Identifier() == "direct")
        {
            // version is now the url to the file
            version = id;

            id = utils::Split(utils::Split(id, "/").back(), ".").front();
            name = utils::Prompt<std::string>("Mod Name", id);
        }

        auto modData = globals::Platform->GetModData(name, version);
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

    void Mod::Download(ModData& mod)
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

        if (mod.download.sha1.empty() || mod.download.sha512.empty())
        {
            const std::filesystem::path modsPath = "mods";
            if (!std::filesystem::exists(modsPath))
            {
                std::filesystem::create_directory(modsPath);
            }

            SHA1 sha1;
            SHA3 sha3{SHA3::Bits512};

            const std::string content = utils::ReadFile(modPath);
            const std::string sha1Hash = sha1(content);
            const std::string sha512Hash = sha3(content);

            mod.download.sha1 = sha1Hash;
            mod.download.sha512 = sha512Hash;
            mod.download.size = std::filesystem::file_size(modPath);

            const nlohmann::json json = mod;
            utils::WriteFile(modsPath / (mod.slug + ".pm"), json.dump(4));
        }
    }
}
