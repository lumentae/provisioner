#include "NewCommand.h"

namespace provisioner::commands
{
    void NewCommand::Register(CLI::App& app)
    {
        auto* sub = app.add_subcommand("new", "Create a new project");
        const auto opt = std::make_shared<Options>();

        sub->add_option("name", opt->name)->required();
        sub->callback([opt]()
        {
            Execute(opt);
        });
    }

    void NewCommand::Execute(const std::shared_ptr<Options>& options)
    {
        // TODO: Create project
    }
}
