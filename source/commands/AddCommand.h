#pragma once
#include <string>
#include <CLI/App.hpp>

namespace provisioner::commands
{
    class AddCommand final
    {
    public:
        struct Options
        {
            std::string name;
            std::string platform;
        };

        AddCommand() = default;
        ~AddCommand() = default;
        static void Register(CLI::App* sub);
        static void Execute(const std::shared_ptr<Options>& options);
    };
}