#pragma once
#include <string>

#include "project/mods/ModData.h"

namespace provisioner::platform
{
    class IPlatform
    {
    public:
        virtual ~IPlatform() = default;
        virtual project::mods::ModData GetModData(const std::string& id, const std::string& version) = 0;

        virtual std::string GetLatestVersion(const std::string& id) = 0;
        virtual void Download(const std::string& id, const std::string& version) = 0;
    };
}