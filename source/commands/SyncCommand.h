#pragma once
#include <string>
#include <CLI/App.hpp>

#include "project/Project.h"

namespace provisioner::commands
{
    class SyncCommand final
    {
    public:
        struct Options
        {
            std::string name;
        };

        SyncCommand() = default;
        ~SyncCommand() = default;
        static void Register(CLI::App* sub);
        static void Execute(const std::shared_ptr<Options>& options);
    };
}