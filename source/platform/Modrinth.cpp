#include "Modrinth.h"

#include <nlohmann/json.hpp>
#include <utils/File.h>
#include <utils/Prompt.h>
#include <Common.h>

#include "Globals.h"
#include "project/Project.h"
#include "utils/String.h"

namespace provisioner::platform
{
    void Modrinth::Download(const std::string& id, const std::string& version)
    {
        const std::filesystem::path modsPath = "mods";
        if (!std::filesystem::exists(modsPath))
        {
            std::filesystem::create_directory(modsPath);
        }

        const auto url = "https://api.modrinth.com/v2/project/" + id + "/version/" + version;
        const auto body = utils::FetchUrl(url);

        nlohmann::json file = nlohmann::json::parse(body)[0]["files"][0];
        const std::string download_url = file["download_url"];
        const std::string filename = file["filename"];

        spdlog::info("Downloading {} from Modrinth", filename);
        utils::DownloadFile(download_url, modsPath / filename);
    }

    project::mods::ModData Modrinth::GetModData(const std::string& id, const std::string& version)
    {
        const auto modUrl = "https://api.modrinth.com/v2/project/" + id;
        const auto modBody = utils::FetchUrl(modUrl);
        nlohmann::json modJson = nlohmann::json::parse(modBody);

        const auto versionUrl = modUrl + "/version/" + version;
        const auto versionBody = utils::FetchUrl(versionUrl);
        nlohmann::json versionJson = nlohmann::json::parse(versionBody);
        nlohmann::json fileJson = versionJson["files"][0];

        project::mods::ModData modData;
        modData.name = modJson["title"];
        modData.id = id;
        modData.slug = modJson["slug"];
        modData.platform = "modrinth";

        modData.download.url = fileJson["url"];
        modData.download.size = fileJson["size"];
        modData.download.sha1 = fileJson["hashes"]["sha1"];
        modData.download.sha512 = fileJson["hashes"]["sha512"];

        modData.update.id = id;
        modData.update.version = version;

        modData.requirements = versionJson["dependencies"];

        return modData;
    }

    std::string Modrinth::GetLatestVersion(const std::string& id)
    {
        REQUIRE_PROJECT_LOAD();

        const auto modUrl = std::format(
            R"(https://api.modrinth.com/v2/project/{}/version?loaders=%5B%22{}%22%5D&game_versions=%5B%22{}%22%5D&featured=true)",
            id,
            project.mData.minecraft.type,
            project.mData.allowedVersionsString.second
        );

        const auto modBody = utils::FetchUrl(modUrl);
        if (modBody.empty())
            return "";

        nlohmann::json modJson = nlohmann::json::parse(modBody);
        if (modJson.empty())
            return "";

        std::string latestVersion = modJson[0]["id"];
        std::string latestVersionReadable = modJson[0]["version_number"];

        return latestVersion;
    }

    std::string Modrinth::Search(const std::string& query)
    {
        REQUIRE_PROJECT_LOAD();

        auto allowedVersionsCopy = project.mData.allowedVersions;
        allowedVersionsCopy.emplace_back(project.mData.minecraft.version);

        std::string versions;
        for (const auto& allowedVersion : allowedVersionsCopy)
        {
            versions += "\"versions:" + allowedVersion + "\",";
        }
        versions.pop_back();

        const auto modUrl = std::format(
            R"(https://api.modrinth.com/v2/search?query={}&facets=%5B%5B%22project_type%3Amod%22%5D%2C%5B{}%5D%2C%5B%22categories%3A{}%22%5D%5D&limit=5)",
            utils::EncodeUrl(query),
            utils::EncodeUrl(versions),
            project.mData.minecraft.type
        );

        const auto modBody = utils::FetchUrl(modUrl);
        nlohmann::json modJson = nlohmann::json::parse(modBody);

        const auto hits = modJson["hits"];
        if (hits.empty())
        {
            throw std::runtime_error(std::format("No mods found for {}", query));
        }
        if (hits.size() == 1)
        {
            return hits[0]["slug"];
        }

        spdlog::info("Found mods:");
        auto index = 1;
        for (const auto& hit : hits)
        {
            spdlog::info("[{}] {} ({})\n\t{}", index, hit["title"].get<std::string>(), hit["slug"].get<std::string>(),
                         hit["description"].get<std::string>());
            index++;
        }

        const auto selected = utils::Prompt<int>("Select a mod", 1);
        if (selected < 1 || selected > hits.size())
        {
            throw std::runtime_error("Invalid selection");
        }

        spdlog::info("Selected mod: {}", hits[selected - 1]["title"].get<std::string>());

        const auto& hit = hits[selected - 1];
        return hit["slug"];
    }
}
