#pragma once
#include <string>
#include <fstream>
#include <stdexcept>
#include <nlohmann/json.hpp>
#include <utility>

#include "ProjectData.h"

namespace provisioner::project
{
    struct MinecraftConfig
    {
        std::string version;
        std::string type;
    };

    class Project
    {
    public:
        explicit Project(std::string path) : mJsonPath(std::move(path))
        {
            Load();
        }

        void Load();
        void Save();

    private:
        std::string mJsonPath;

        ProjectData mData;
    };
}