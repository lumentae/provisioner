#pragma once
#include <string>
#include <CLI/App.hpp>

namespace provisioner::commands
{
    class CompileCommand final
    {
    public:
        struct Options
        {
            std::filesystem::path folder = std::filesystem::current_path() / "compiled";
        };

        CompileCommand() = default;
        ~CompileCommand() = default;
        static void Register(CLI::App* sub);
        static void Execute(const std::shared_ptr<Options>& options);
    };
}