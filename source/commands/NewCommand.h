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
        };

        NewCommand() = default;
        ~NewCommand() = default;
        static void Register(CLI::App& app);
        static void Execute(const std::shared_ptr<Options>& options);
    };
}
