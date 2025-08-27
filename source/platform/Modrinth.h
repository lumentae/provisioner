#pragma once
#include "IPlatform.h"

namespace provisioner::platform
{
    class Modrinth final : public IPlatform
    {
    public:
        void Download(const std::string& id, const std::string& version) override;
        project::mods::ModData GetModData(const std::string& id, const std::string& version) override;
        std::string GetLatestVersion(const std::string& id) override;
        std::string Search(const std::string& query) override;
    };
}