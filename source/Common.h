#pragma once

#include <string>

#define REGISTER_COMMAND(name, description) \
    auto* command##name = app.add_subcommand(provisioner::utils::ToLower(#name), description); \
    provisioner::commands::name##Command::Register(command##name);