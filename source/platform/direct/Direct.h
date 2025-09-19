#pragma once
#include "platform/IPlatform.h"

namespace provisioner::platform::direct
{
    class Direct final : public IPlatform
    {
    public:
        void Download(const std::string& id, const std::string& version) override;
        project::mods::ModData GetModData(const std::string& name, const std::string& url) override;
        std::string GetLatestVersion(const std::string& id) override;
        std::string Search(const std::string& query) override;
    };
}