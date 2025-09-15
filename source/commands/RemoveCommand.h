#pragma once
#include <string>
#include <CLI/App.hpp>

namespace provisioner::commands
{
    class RemoveCommand final
    {
    public:
        struct Options
        {
            std::string name;
        };

        RemoveCommand() = default;
        ~RemoveCommand() = default;
        static void Register(CLI::App* sub);
        static void Execute(const std::shared_ptr<Options>& options);
    };
}