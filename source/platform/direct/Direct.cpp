#include "Direct.h"

namespace provisioner::platform::direct
{
    void Direct::Download(const std::string& id, const std::string& version)
    {
    }

    project::mods::ModData Direct::GetModData(const std::string& name, const std::string& url)
    {
        project::mods::ModData modData;
        modData.name = name;
        modData.id = name;
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