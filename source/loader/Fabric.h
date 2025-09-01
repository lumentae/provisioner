#pragma once
#include "ILoader.h"

namespace provisioner::loader
{
    class Fabric final : public ILoader
    {
    public:
        void Download(const std::filesystem::path& path) override;
        static std::string GetLatestInstaller();
    };
}