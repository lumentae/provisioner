#include "Project.h"

#include "mods/Mod.h"
#include "utils/File.h"

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

        mData = json.get<ProjectData>();
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

    void Project::Compile(const std::filesystem::path& path)
    {
        if (std::filesystem::exists(path))
            std::filesystem::remove_all(path);

        std::filesystem::create_directory(path);

        for (const auto& include : mData.includes)
        {
            const auto includePath = std::filesystem::path(include);
            if (!std::filesystem::exists(includePath))
                continue;

            const auto destPath = path / includePath;
            std::filesystem::copy_file(includePath, destPath, std::filesystem::copy_options::overwrite_existing);
        }

        const auto modsPath = path / "mods";
        std::filesystem::create_directory(modsPath);
        for (const auto& file : utils::GetFilesByExtension(std::filesystem::current_path() / "mods", "pm"))
        {
            std::string content = utils::ReadFile(file);
            mods::ModData modData = nlohmann::json::parse(content);

            mods::Mod::Download(modData);
        }
    }
}