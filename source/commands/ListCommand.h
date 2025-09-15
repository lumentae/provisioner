#pragma once
#include <string>
#include <CLI/App.hpp>

namespace provisioner::commands
{
    class ListCommand final
    {
    public:
        struct Options
        {
        };

        ListCommand() = default;
        ~ListCommand() = default;
        static void Register(CLI::App* sub);
        static void Execute(const std::shared_ptr<Options>& options);
    };
}