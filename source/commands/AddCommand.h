#pragma once
#include <string>
#include <CLI/App.hpp>

#include "project/Project.h"

namespace provisioner::commands
{
    class AddCommand final
    {
    public:
        struct Options
        {
            DECLARE_DEFAULT_OPTIONS();
            std::string name;
            std::string version;
        };

        AddCommand() = default;
        ~AddCommand() = default;
        static void Register(CLI::App* sub);
        static void Execute(const std::shared_ptr<Options>& options);
    };
}