#include "ExportCommand.h"

#include "project/Project.h"
#include "project/mods/Mod.h"
#include "utils/File.h"
#include "utils/String.h"

#include <libzippp/libzippp.h>

namespace provisioner::commands
{
    void ExportCommand::Register(CLI::App* sub)
    {
        const auto opt = std::make_shared<Options>();

        sub->add_option("path", opt->path, "Where to save the exported file to")->allow_extra_args(true);
        sub->add_option("-t,--type", opt->type, "The type of export to do")->default_val("mrpack");
        sub->callback([opt]()
        {
            Execute(opt);
        });
    }

    void ExportCommand::Execute(const std::shared_ptr<Options>& options)
    {
        REQUIRE_PROJECT()
        ENSURE_STRING(options->type, "mrpack");

        auto exportFile = project.mData.name + "." + options->type;
        if (!options->path.empty())
        {
            exportFile = options->path;
        }

        project.Export(options->type, exportFile);
    }
}