#include "Direct.h"
#include "sha1.h"
#include "sha3.h"
#include "project/mods/Mod.h"
#include "utils/File.h"

namespace provisioner::platform::direct
{
    void Direct::Download(const std::string& name, const std::string& url)
    {
        const std::filesystem::path modsPath = "mods";
        if (!std::filesystem::exists(modsPath))
        {
            std::filesystem::create_directory(modsPath);
        }

        spdlog::info("Downloading {} from direct URL", name);
        utils::DownloadFile(url, modsPath);

        if (project::mods::ModData modData = nlohmann::json::parse((modsPath / (name + ".pm")).string()); modData.
            download.sha1.empty())
        {
            SHA1 sha1;
            SHA3 sha3{SHA3::Bits512};

            const std::string content = utils::ReadFile(modsPath);
            const std::string sha1Hash = sha1(content);
            const std::string sha512Hash = sha3(content);

            modData.download.sha1 = sha1Hash;
            modData.download.sha512 = sha512Hash;
            modData.download.size = std::filesystem::file_size(modsPath);

            const nlohmann::json json = modData;
            utils::WriteFile(modsPath, json.dump(4));
        }
    }

    project::mods::ModData Direct::GetModData(const std::string& name, const std::string& url)
    {
        project::mods::ModData modData;
        modData.name = name;
        modData.id = name;
        modData.slug = name;
        modData.platform = "direct";

        modData.download.url = url;
        modData.download.size = 0;
        modData.download.sha1 = "";
        modData.download.sha512 = "";

        modData.update.id = name;
        modData.update.version = "";

        modData.requirements = {};

        return modData;
    }

    std::string Direct::GetLatestVersion(const std::string& id)
    {
        return "";
    }

    std::string Direct::Search(const std::string& query)
    {
        return query;
    }
}
