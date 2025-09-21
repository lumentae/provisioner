#include "Neoforge.h"

#include "project/Project.h"
#include "utils/File.h"
#include "utils/String.h"

namespace provisioner::loader
{
    void Neoforge::Download(const std::filesystem::path& path)
    {
        REQUIRE_PROJECT()

        auto installerVersion = project.mData.minecraft.loaderVersion;
        if (installerVersion == "latest")
            installerVersion = GetLatestInstaller();

        project.mData.minecraft.loaderVersion = installerVersion;
        project.Save();

        const std::filesystem::path cachePath = ".cache";
        const auto cacheFile = cachePath / ("neoforge-" + installerVersion + ".jar");

        if (!std::filesystem::exists(cacheFile))
        {
            const auto installerUrl = std::format(
                "https://maven.neoforged.net/releases/net/neoforged/neoforge/{0}/neoforge-{0}-installer.jar",
                installerVersion
            );

            spdlog::info("Downloading Neoforge {} installer from {}", installerVersion, installerUrl);
            utils::DownloadFile(installerUrl, cacheFile);
            std::filesystem::copy_file(cacheFile, path, std::filesystem::copy_options::overwrite_existing);
        }

        spdlog::info("Using cached Neoforge installer at {}", cacheFile.string());
        std::filesystem::copy_file(cacheFile, path, std::filesystem::copy_options::overwrite_existing);

        auto installCommand = std::format("java -jar {} --install-server {}", path.string(),
                                          path.parent_path().string());
        spdlog::info("Running {}", installCommand);
        std::system(installCommand.c_str());
        std::filesystem::remove(path.parent_path() / "server.jar");
        std::filesystem::remove(path.parent_path().parent_path() / "server.jar.log");
    }

    std::string Neoforge::GetLatestInstaller()
    {
        REQUIRE_PROJECT()

        const auto body = utils::FetchUrl(
            "https://maven.neoforged.net/api/maven/versions/releases/net%2Fneoforged%2Fneoforge");

        const auto extractVersion = [](const std::string& version)
        {
            const auto parts = utils::Split(version, ".");
            return "1." + parts[0] + "." + parts[1];
        };

        std::vector<std::string> useableVersions{};
        for (nlohmann::json json = nlohmann::json::parse(body); std::string version : json["versions"])
        {
            if (const auto mcVersion = extractVersion(version); mcVersion == project.mData.minecraft.version)
                useableVersions.push_back(version);
        }

        return useableVersions.back();
    }
}