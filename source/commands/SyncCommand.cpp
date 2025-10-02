#include "SyncCommand.h"

#include "project/Project.h"
#include "project/mods/Mod.h"

namespace provisioner::commands
{
    void SyncCommand::Register(CLI::App* sub)
    {
        const auto opt = std::make_shared<Options>();

        sub->add_option("name", opt->name)->required()->allow_extra_args(true);
        sub->callback([opt]()
        {
            Execute(opt);
        });
    }

    void SyncCommand::Execute(const std::shared_ptr<Options>& options)
    {
        REQUIRE_PROJECT();

        project.Sync(options->name);
    }
}