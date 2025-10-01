#include "Project.h"

#include <execution>
#include <libzippp.h>

#include "loader/Fabric.h"
#include "loader/Neoforge.h"
#include "platform/modrinth/ModrinthIndexData.h"
#include "mods/Mod.h"
#include "utils/File.h"
#include "utils/String.h"

namespace provisioner::project
{
    void Project::Load()
    {
        std::ifstream file(mJsonPath);
        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open " + mJsonPath.string());
        }

        nlohmann::json json;
        file >> json;

        mData = json;

        auto allowedVersionsCopy = mData.allowedVersions;
        allowedVersionsCopy.emplace_back(mData.minecraft.version);

        mData.allowedVersionsString.first = utils::Join(allowedVersionsCopy, ",");
        mData.allowedVersionsString.second = utils::Join(allowedVersionsCopy, "%22,%22");

        ENSURE_STRING(mData.minecraft.type, "fabric", "neoforge", "vanilla");

        if (mData.minecraft.type == "neoforge")
            mLoader = std::make_shared<loader::Neoforge>();
        else
            mLoader = std::make_shared<loader::Fabric>();
    }

    void Project::Save()
    {
        std::ofstream file(mJsonPath);
        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open " + mJsonPath.string());
        }

        const nlohmann::json json = mData;
        file << json.dump(4);
    }

    void Project::Compile(const std::filesystem::path& path, bool skipSetup) const
    {
        if (std::filesystem::exists(path))
            std::filesystem::remove_all(path);

        std::filesystem::create_directory(path);

        for (const auto& include : mData.includes)
        {
            const auto includePath = std::filesystem::path(include);
            if (!std::filesystem::exists(includePath))
            {
                spdlog::warn("Include {} does not exist", include);
                continue;
            }

            const auto destPath = path / includePath;
            if (std::filesystem::is_regular_file(includePath))
                std::filesystem::copy_file(includePath, destPath, std::filesystem::copy_options::overwrite_existing);
            else
                std::filesystem::copy(includePath, destPath,
                                      std::filesystem::copy_options::recursive |
                                      std::filesystem::copy_options::overwrite_existing);
        }

        const auto modsPath = path / "mods";
        std::filesystem::create_directory(modsPath);
        for (const auto& file : utils::GetFilesByExtension(std::filesystem::current_path() / "mods", "pm"))
        {
            std::string content = utils::ReadFile(file);
            mods::ModData modData = nlohmann::json::parse(content);

            mods::Mod::Download(modData);

            const std::string modFileName = (modData.slug + ".jar");
            const std::filesystem::path cachePath = ".cache";
            const std::filesystem::path modPath = cachePath / modFileName;
            std::filesystem::copy_file(modPath, modsPath / modFileName,
                                       std::filesystem::copy_options::overwrite_existing);
        }

        mLoader->Download(path / "server.jar", skipSetup);

        spdlog::info("Compiled project to {}", path.string());
    }

    void Project::Export(const std::string& type, const std::filesystem::path& path) const
    {
        const auto tempPath = std::filesystem::temp_directory_path() / "provisioner_temp";
        Compile(tempPath, true);

        if (type == "mrpack")
        {
            if (std::filesystem::exists(path))
                std::filesystem::remove(path);

            libzippp::ZipArchive zip(path.string());
            if (!zip.open(libzippp::ZipArchive::Write))
            {
                std::filesystem::remove_all(tempPath);
                throw std::runtime_error("Failed to open zip file");
            }

            auto overridesPath = std::filesystem::path("overrides");
            platform::modrinth::ModrinthIndexData indexData;
            indexData.name = mData.name;

            for (const auto& file : utils::GetFilesByExtension(std::filesystem::current_path() / "mods", "pm"))
            {
                std::string fileContent = utils::ReadFile(file);
                mods::ModData modData = nlohmann::json::parse(fileContent);

                if (modData.platform != "modrinth")
                {
                    auto modFile = modData.slug + ".jar";
                    if (auto modPath = overridesPath / "mods" / modFile; !zip.addFile(
                        modPath.generic_string(), ".cache/" + modFile))
                    {
                        std::filesystem::remove_all(tempPath);
                        throw std::runtime_error("Failed to add " + modData.slug);
                    }
                    continue;
                }

                platform::modrinth::ModrinthIndexFile indexFile = platform::modrinth::GetIndexFileFromModData(modData);
                indexData.files.emplace_back(indexFile);
            }

            auto loaderType = mData.minecraft.type;
            if (loaderType == "fabric" || loaderType == "quilt")
            {
                loaderType += "-loader";
            }

            indexData.dependencies.emplace("minecraft", mData.minecraft.version);
            indexData.dependencies.emplace(loaderType, mData.minecraft.loaderVersion);

            std::string content = nlohmann::json(indexData).dump(4) + "\n";
            if (!zip.addData("modrinth.index.json", content.c_str(), content.size()))
            {
                std::filesystem::remove_all(tempPath);
                throw std::runtime_error("Failed to add modrinth.index.json");
            }

            for (const auto& include : mData.includes)
            {
                for (const auto& file : utils::GetFilesByExtension(include, "", true))
                {
                    auto relativePath = std::filesystem::relative(file, include).string();
                    if (relativePath.back() == '.')
                        relativePath.pop_back();

                    auto zipPath = (overridesPath / include / relativePath).generic_string();
                    if (zipPath.back() == '/')
                        zipPath.pop_back();

                    spdlog::info("Adding {} as {}", file.string(), zipPath);

                    if (!zip.addFile(zipPath, file.string()))
                    {
                        std::filesystem::remove_all(tempPath);
                        throw std::runtime_error("Failed to add " + file.string());
                    }
                }
            }

            zip.close();
            spdlog::info("Exported project to {}", path.string());
            std::filesystem::remove_all(tempPath);
            return;
        }
        if (type == "zip")
        {
            if (std::filesystem::exists(path))
                std::filesystem::remove(path);

            libzippp::ZipArchive zip(path.string());
            if (!zip.open(libzippp::ZipArchive::Write))
            {
                std::filesystem::remove_all(tempPath);
                throw std::runtime_error("Failed to open zip file");
            }

            for (const auto& file : utils::GetFilesByExtension(tempPath, "", true))
            {
                auto relativePath = std::filesystem::relative(file, tempPath).string();
                if (relativePath.back() == '.')
                    relativePath.pop_back();

                spdlog::info("Adding {} as {}", file.string(), relativePath);
                if (!zip.addFile(relativePath, file.string()))
                {
                    std::filesystem::remove_all(tempPath);
                    throw std::runtime_error("Failed to add " + file.string());
                }
            }

            zip.close();
            spdlog::info("Exported project to {}", path.string());
            std::filesystem::remove_all(tempPath);
            return;
        }

        std::filesystem::remove_all(tempPath);
        throw std::runtime_error("Unsupported export type: " + type);
    }
}
