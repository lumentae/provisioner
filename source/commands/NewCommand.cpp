#include "NewCommand.h"

#include "loader/Fabric.h"
#include "loader/Neoforge.h"
#include "project/Project.h"
#include "utils/Prompt.h"

namespace provisioner::commands
{
    void NewCommand::Register(CLI::App* sub)
    {
        const auto opt = std::make_shared<Options>();

        sub->add_option("name", opt->name)->required()->allow_extra_args(true);
        sub->add_flag("-f,--force", opt->force, "Force creation even if the project already exists")->
             default_val(false);
        sub->callback([opt]()
        {
            Execute(opt);
        });
    }

    void NewCommand::Execute(const std::shared_ptr<Options>& options)
    {
        const auto projectPath = std::filesystem::current_path() / options->name;
        if (std::filesystem::exists(projectPath) && !options->force)
            throw std::runtime_error("Project already exists");

        std::filesystem::create_directory(projectPath);

        const project::Project projectInstance(projectPath / "project.json");
        auto& project = project::Project::GetInstance();
        project.mData = projectInstance.mData;
        project.mJsonPath = projectInstance.mJsonPath;

        project.mData.name = options->name;

        project.mData.minecraft.version = utils::Prompt<std::string>("Minecraft Version (e.g. 1.21.1)", "1.21.8");
        project.mData.minecraft.type = utils::ToLower(
            utils::Prompt<std::string>("Loader (fabric, neoforge, vanilla)", "vanilla"));

        ENSURE_STRING(project.mData.minecraft.type, "fabric", "neoforge", "vanilla");

        if (project.mData.minecraft.type == "fabric")
        {
            const auto latestLoader = loader::Fabric::GetLatestLoader();
            project.mData.minecraft.loaderVersion = utils::Prompt<std::string>(
                "Loader Version (e.g. 0.17.2)", latestLoader);
        }
        else if (project.mData.minecraft.type == "neoforge")
        {
            const auto latestLoader = loader::Neoforge::GetLatestInstaller();
            project.mData.minecraft.loaderVersion = utils::Prompt<std::string>(
                "Loader Version (e.g. 21.1.64)", latestLoader);
        }

        project.Save();

        spdlog::info("Project '{}' created successfully", options->name);
    }
}