#include <iostream>
#include <CLI/CLI.hpp>
#include <curl/curl.h>

#include "Common.h"
#include "commands/NewCommand.h"
#include "commands/AddCommand.h"
#include "commands/IncludeCommand.h"
#include "commands/CompileCommand.h"
#include "commands/UpdateCommand.h"
#include "project/Project.h"
#include "utils/String.h"

/* TODO:
 * Implement plugin support
 * Add commands for server control
 * Add export command (for .mrpack)
 */

int main(const int argc, char** argv)
{
#ifndef DEBUG
    try
    {
#endif
        if (std::filesystem::exists("project.json"))
        {
            auto& project = provisioner::project::Project::GetInstance();
            project.Load();
        }

        curl_global_init(CURL_GLOBAL_DEFAULT);

        CLI::App app{"Provisioner CLI"};
        app.validate_optional_arguments();
        app.validate_positionals();
        argv = app.ensure_utf8(argv);

        REGISTER_COMMAND(New, "Create a new project")
        REGISTER_COMMAND(Add, "Adds a mod to a project")
        REGISTER_COMMAND(Include, "Includes a file in a project")
        REGISTER_COMMAND(Compile, "Compiles the project into a server")
        REGISTER_COMMAND(Update, "Upgrades mods in a project")
        REGISTER_ALIAS(Upgrade, Update)

        app.require_subcommand(1);

        CLI11_PARSE(app, argc, argv);
#ifndef DEBUG
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
#endif
    return 0;
}