#include "RemoveCommand.h"

#include "project/Project.h"
#include "project/mods/Mod.h"
#include "utils/File.h"

namespace provisioner::commands
{
    void RemoveCommand::Register(CLI::App* sub)
    {
        const auto opt = std::make_shared<Options>();

        sub->add_option("name", opt->name)->required(true)->allow_extra_args(true);
        sub->callback([opt]()
        {
            Execute(opt);
        });
    }

    void RemoveCommand::Execute(const std::shared_ptr<Options>& options)
    {
        REQUIRE_PROJECT()

        project::mods::Mod::Remove(options->name);
    }
}