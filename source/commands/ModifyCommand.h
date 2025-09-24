#pragma once
#include <string>
#include <CLI/App.hpp>

namespace provisioner::commands
{
    class ModifyCommand final
    {
    public:
        struct Options
        {
        };

        ModifyCommand() = default;
        ~ModifyCommand() = default;
        static void Register(CLI::App* sub);
        static void Execute(const std::shared_ptr<Options>& options);
    };
}