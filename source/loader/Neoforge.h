#pragma once
#include "ILoader.h"

namespace provisioner::loader
{
    class Neoforge final : public ILoader
    {
    public:
        void Download(const std::filesystem::path& path, bool skipSetup = true) override;
        static std::string GetLatestInstaller();
    };
}