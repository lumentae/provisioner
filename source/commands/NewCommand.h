#pragma once
#include <string>
#include <CLI/App.hpp>

namespace provisioner::commands
{
    class NewCommand final
    {
    public:
        struct Options
        {
            std::string name;
            bool force;
        };

        NewCommand() = default;
        ~NewCommand() = default;
        static void Register(CLI::App* sub);
        static void Execute(const std::shared_ptr<Options>& options);
    };
}