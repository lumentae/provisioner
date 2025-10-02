#pragma once
#include <nlohmann/json.hpp>
#include <utility>

#include "ProjectData.h"
#include "loader/ILoader.h"
#include "utils/String.h"
#include "Globals.h"
#include "platform/modrinth/Modrinth.h"
#include "platform/direct/Direct.h"

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
        void Compile(const std::filesystem::path& path, bool skipSetup = false) const;
        void Export(const std::string& type, const std::filesystem::path& path) const;
        void Sync(const std::filesystem::path& path) const;

        ProjectData mData;
        std::shared_ptr<loader::ILoader> mLoader;
        std::filesystem::path mJsonPath;
    };
}

#define REQUIRE_PROJECT_LOAD() \
    auto& project = project::Project::GetInstance(); \
    if (project.mData.name.empty() || !std::filesystem::exists("project.json")) \
    { \
        project.Load(); \
    }

#define REQUIRE_PROJECT() \
    auto& project = project::Project::GetInstance(); \
    if (project.mData.name.empty() || !std::filesystem::exists("project.json")) \
    { \
        throw std::runtime_error("No project found. Please run 'provisioner new' to create a new project. Please check if you're in a directory with a project.json!"); \
    }

#define ENSURE_STRING(val, ...) \
    do { \
        bool __ok = false; \
        for (const auto __s : {__VA_ARGS__}) { \
            if (val == __s) { \
                __ok = true; \
                break; \
            } \
        } \
        if (!__ok) { \
            std::vector<std::string> allowedValues = {__VA_ARGS__}; \
            throw std::runtime_error(std::format("{} failed, allowed values are [{}], provided was {}", #val, utils::Join(allowedValues, ", "), val)); \
        } \
    } while (0)

#define DEFINE_DEFAULT_OPTIONS() \
    sub->add_option("-p,--platform", opt->platform, "Which platform to download from")->default_val("modrinth")

#define DECLARE_DEFAULT_OPTIONS() \
    std::string platform

#define IMPLEMENT_DEFAULT_OPTIONS() \
    ENSURE_STRING(options->platform, "modrinth", "direct"); \
    if (options->platform == "modrinth") \
    { \
        globals::Platform = std::make_shared<provisioner::platform::modrinth::Modrinth>(); \
    } \
    else if (options->platform == "direct") \
    { \
        globals::Platform = std::make_shared<provisioner::platform::direct::Direct>(); \
    }