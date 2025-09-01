#pragma once
#include <filesystem>

namespace provisioner::loader
{
    class ILoader
    {
    public:
        virtual ~ILoader() = default;
        virtual void Download(const std::filesystem::path& path) = 0;
    };
}
