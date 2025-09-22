#pragma once
#include <string>
#include <CLI/App.hpp>

#include "project/Project.h"

namespace provisioner::commands
{
    class UpdateCommand final
    {
    public:
        struct Options
        {
            DECLARE_DEFAULT_OPTIONS();
            std::string name;
            bool force;
        };

        UpdateCommand() = default;
        ~UpdateCommand() = default;
        static void Register(CLI::App* sub);
        static void Execute(const std::shared_ptr<Options>& options);
    };
}