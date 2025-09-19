#include "AddCommand.h"

#include "project/Project.h"
#include "project/mods/Mod.h"

namespace provisioner::commands
{
    void AddCommand::Register(CLI::App* sub)
    {
        const auto opt = std::make_shared<Options>();

        sub->add_option("name", opt->name)->required()->allow_extra_args(true);
        sub->add_option("-v,--version", opt->version, "The version to download")->default_val("latest");
        DEFINE_DEFAULT_OPTIONS();

        sub->callback([opt]()
        {
            Execute(opt);
        });
    }

    void AddCommand::Execute(const std::shared_ptr<Options>& options)
    {
        REQUIRE_PROJECT()
        IMPLEMENT_DEFAULT_OPTIONS();

        project::mods::Mod::Add(options->name, options->version);
    }
}