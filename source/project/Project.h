#pragma once
#include <string>
#include <fstream>
#include <stdexcept>
#include <nlohmann/json.hpp>
#include <utility>

#include "ProjectData.h"

namespace provisioner::project
{
    class Project
    {
    public:
        explicit Project(std::filesystem::path path) : mJsonPath(std::move(path))
        {
        }

        void Load();
        void Save();

        ProjectData mData;

    private:
        std::filesystem::path mJsonPath;
    };
}