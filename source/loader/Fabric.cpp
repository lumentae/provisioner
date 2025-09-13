#include "Fabric.h"

#include "project/Project.h"
#include "utils/File.h"

namespace provisioner::loader
{
    void Fabric::Download(const std::filesystem::path& path)
    {
        REQUIRE_PROJECT()

        const auto latestInstallerVersion = GetLatestInstaller();
        const std::filesystem::path cachePath = ".cache";
        const auto cacheFile = cachePath / ("fabric-" + latestInstallerVersion + ".jar");

        if (std::filesystem::exists(cacheFile))
        {
            spdlog::info("Using cached Fabric installer at {}", cacheFile.string());
            std::filesystem::copy_file(cacheFile, path, std::filesystem::copy_options::overwrite_existing);
            return;
        }

        auto loaderVersion = project.mData.minecraft.loaderVersion;
        if (project.mData.minecraft.loaderVersion == "latest")
        {
            loaderVersion = GetLatestLoader();
        }

        const auto metaUrl = std::format("https://meta.fabricmc.net/v2/versions/loader/{}/{}/{}/server/jar",
                                         project.mData.minecraft.version,
                                         loaderVersion,
                                         latestInstallerVersion
        );

        spdlog::info("Downloading Fabric {} installer from {}", latestInstallerVersion, metaUrl);
        utils::DownloadFile(metaUrl, cacheFile);
        std::filesystem::copy_file(cacheFile, path, std::filesystem::copy_options::overwrite_existing);
    }

    std::string Fabric::GetLatestInstaller()
    {
        const auto body = utils::FetchUrl("https://meta.fabricmc.net/v2/versions/installer");

        nlohmann::json json = nlohmann::json::parse(body);
        return json[0]["version"];
    }

    std::string Fabric::GetLatestLoader()
    {
        const auto body = utils::FetchUrl("https://meta.fabricmc.net/v2/versions/loader");

        nlohmann::json json = nlohmann::json::parse(body);
        return json[0]["version"];
    }
}
