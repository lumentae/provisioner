#include "NewCommand.h"

#include "project/Project.h"

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
        if (std::filesystem::exists(projectPath))
            throw std::runtime_error("Project already exists");

        std::filesystem::create_directory(projectPath);

        project::Project project(projectPath / "project.json");
        project.mData.name = options->name;
        project.Save();

        spdlog::info("Project '{}' created successfully", options->name);
    }
}