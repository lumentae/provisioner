#include "IncludeCommand.h"

#include "project/Project.h"
#include "project/mods/Mod.h"

namespace provisioner::commands
{
    void IncludeCommand::Register(CLI::App* sub)
    {
        const auto opt = std::make_shared<Options>();

        sub->add_option("name", opt->name)->required()->allow_extra_args(true);
        sub->add_flag("-r,--remove", opt->remove, "Remove the include instead of adding it");
        sub->callback([opt]()
        {
            Execute(opt);
        });
    }

    void IncludeCommand::Execute(const std::shared_ptr<Options>& options)
    {
        REQUIRE_PROJECT()

        if (options->remove)
            std::erase(project.mData.includes, options->name);
        else
            project.mData.includes.push_back(options->name);

        spdlog::info("{} {}", options->remove ? "Removed" : "Added", options->name);
        project.Save();
    }
}