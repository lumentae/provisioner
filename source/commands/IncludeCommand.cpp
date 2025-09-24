#include "IncludeCommand.h"

#include "project/Project.h"
#include "project/mods/Mod.h"

namespace provisioner::commands
{
    void IncludeCommand::Register(CLI::App* sub)
    {
        const auto opt = std::make_shared<Options>();

        sub->add_option("name", opt->name)->required()->allow_extra_args(true);
        sub->callback([opt]()
        {
            Execute(opt);
        });
    }

    void IncludeCommand::Execute(const std::shared_ptr<Options>& options)
    {
        REQUIRE_PROJECT()

        // TODO: Remove functionality
        project.mData.includes.push_back(options->name);
        project.Save();
    }
}