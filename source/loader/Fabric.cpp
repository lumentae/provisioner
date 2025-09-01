#include "Fabric.h"

#include "project/Project.h"
#include "utils/File.h"

namespace provisioner::loader
{
    void Fabric::Download(const std::filesystem::path& path)
    {
        REQUIRE_PROJECT()

        const auto latestInstallerVersion = GetLatestInstaller();
        spdlog::info("Downloading Fabric {} installer", latestInstallerVersion);

        const std::filesystem::path cachePath = ".cache";
        const auto cacheFile = cachePath / ("fabric-" + latestInstallerVersion + ".jar");

        const auto metaUrl = std::format("https://meta.fabricmc.net/v2/versions/loader/{}/{}/{}/server/jar",
                                         project.mData.minecraft.type,
                                         project.mData.minecraft.version,
                                         latestInstallerVersion
        );

        utils::DownloadFile(metaUrl, cacheFile);
        std::filesystem::copy_file(cacheFile, path, std::filesystem::copy_options::overwrite_existing);
    }

    std::string Fabric::GetLatestInstaller()
    {
        const auto body = utils::FetchUrl("https://meta.fabricmc.net/v2/versions/installer");

        nlohmann::json json = nlohmann::json::parse(body);
        return json[0]["version"];
    }
}
