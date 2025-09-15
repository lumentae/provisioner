#pragma once
#include <string>
#include <CLI/App.hpp>

namespace provisioner::commands
{
    class ExportCommand final
    {
    public:
        struct Options
        {
            std::string path;
            std::string type;
        };

        ExportCommand() = default;
        ~ExportCommand() = default;
        static void Register(CLI::App* sub);
        static void Execute(const std::shared_ptr<Options>& options);
    };
}
