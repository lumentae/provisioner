#include <iostream>
#include <CLI/CLI.hpp>

#include "Common.h"
#include "commands/NewCommand.h"
#include "utils/String.h"

int main(const int argc, char** argv)
{
    CLI::App app{"Provisioner CLI"};
    app.validate_optional_arguments();
    app.validate_positionals();
    argv = app.ensure_utf8(argv);

    REGISTER_COMMAND(New, "Create a new project")

    app.require_subcommand(1);

    CLI11_PARSE(app, argc, argv);
    return 0;
}