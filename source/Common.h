#pragma once

#include <string>
#include <spdlog/spdlog.h>

#define REGISTER_COMMAND(name, description) \
    auto* command##name = app.add_subcommand(provisioner::utils::ToLower(#name), description); \
    provisioner::commands::name##Command::Register(command##name);

#define REGISTER_ALIAS(pAlias, name) \
    command##name->alias(provisioner::utils::ToLower(#pAlias));