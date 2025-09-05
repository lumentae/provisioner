#include "AddCommand.h"

#include "project/Project.h"
#include "project/mods/Mod.h"

namespace provisioner::commands
{
    void AddCommand::Register(CLI::App* sub)
    {
        const auto opt = std::make_shared<Options>();

        sub->add_option("name", opt->name)->required()->allow_extra_args(true);
        sub->add_flag("-p,--platform", opt->platform, "Which platform to download from")->default_val("modrinth");
        sub->add_option("-v,--version", opt->version, "The version to download")->default_val("latest");
        sub->callback([opt]()
        {
            Execute(opt);
        });
    }

    void AddCommand::Execute(const std::shared_ptr<Options>& options)
    {
        REQUIRE_PROJECT()

        // TODO: Use platform

        project::mods::Mod::Add(options->name, options->version);
    }
}