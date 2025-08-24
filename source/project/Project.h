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
        static Project& GetInstance()
        {
            static Project instance(std::filesystem::path("project.json"));
            return instance;
        }

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

#define REQUIRE_PROJECT \
    auto& project = provisioner::project::Project::GetInstance(); \
    if (project.mData.name.empty()) \
    { \
        throw std::runtime_error("No project found. Please run 'provisioner new' to create a new project."); \
    }