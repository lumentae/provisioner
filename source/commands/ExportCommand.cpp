#include "ExportCommand.h"

#include "project/Project.h"
#include "project/mods/Mod.h"
#include "utils/File.h"

namespace provisioner::commands
{
    void ExportCommand::Register(CLI::App* sub)
    {
        const auto opt = std::make_shared<Options>();

        sub->add_option("path", opt->path, "Where to save the exported file to")->allow_extra_args(true);
        sub->add_flag("-t,--type", opt->type, "The type of export to do")->default_val("mrpack");
        sub->callback([opt]()
        {
            Execute(opt);
        });
    }

    void ExportCommand::Execute(const std::shared_ptr<Options>& options)
    {
        REQUIRE_PROJECT()
        ENSURE_STRING(options->type, "mrpack");

        auto exportFileName = project.mData.name + options->type;
        if (!options->path.empty())
        {
            exportFileName = options->path;
        }

        throw std::runtime_error("Not implemented");
    }
}
