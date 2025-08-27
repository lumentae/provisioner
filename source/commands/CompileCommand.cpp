#include "CompileCommand.h"

#include "project/Project.h"
#include "project/mods/Mod.h"

namespace provisioner::commands
{
    void CompileCommand::Register(CLI::App* sub)
    {
        const auto opt = std::make_shared<Options>();

        sub->add_flag("-f,--folder", opt->folder, "The folder to compile to");
        sub->callback([opt]()
        {
            Execute(opt);
        });
    }

    void CompileCommand::Execute(const std::shared_ptr<Options>& options)
    {
        REQUIRE_PROJECT()

        project.Compile(options->folder);
    }
}