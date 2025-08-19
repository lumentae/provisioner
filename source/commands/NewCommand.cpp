#include "NewCommand.h"

namespace provisioner::commands
{
    void NewCommand::Register(CLI::App* sub)
    {
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
        std::cout << "Creating project " << options->name << std::endl;
    }
}