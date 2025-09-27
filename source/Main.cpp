#include <iostream>
#include <CLI/CLI.hpp>
#include <curl/curl.h>

#include "Common.h"
#include "commands/NewCommand.h"
#include "commands/AddCommand.h"
#include "commands/IncludeCommand.h"
#include "commands/CompileCommand.h"
#include "commands/RemoveCommand.h"
#include "commands/UpdateCommand.h"
#include "commands/ExportCommand.h"
#include "commands/ListCommand.h"
#include "commands/ModifyCommand.h"
#include "project/Project.h"
#include "utils/String.h"

/* TODO:
 * Implement plugin support
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
        REGISTER_COMMAND(Remove, "Removes a mod from a project")
        REGISTER_COMMAND(Export, "Exports a project to a .mrpack file")
        REGISTER_COMMAND(List, "Lists all mods in a project")
        REGISTER_COMMAND(Modify, "Modifies the config files")

        REGISTER_ALIAS(Upgrade, Update)
    REGISTER_ALIAS(Refresh, Update)
    REGISTER_ALIAS(Install, Add)
        REGISTER_ALIAS(Uninstall, Remove)
    REGISTER_ALIAS(Delete, Remove)
    REGISTER_ALIAS(Rm, Remove)
    REGISTER_ALIAS(Create, New)
    REGISTER_ALIAS(Init, New)
    REGISTER_ALIAS(Ls, List)
    REGISTER_ALIAS(Config, Modify)
    REGISTER_ALIAS(Edit, Modify)

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