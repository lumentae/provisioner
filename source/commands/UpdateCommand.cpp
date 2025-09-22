#include "UpdateCommand.h"

#include "project/Project.h"
#include "project/mods/Mod.h"
#include "utils/File.h"

namespace provisioner::commands
{
    void UpdateCommand::Register(CLI::App* sub)
    {
        const auto opt = std::make_shared<Options>();

        sub->add_option("name", opt->name)->allow_extra_args(true);
        sub->add_flag("-f,--force", opt->force, "Force update even if the mod is up to date");
        DEFINE_DEFAULT_OPTIONS();
        sub->callback([opt]()
        {
            Execute(opt);
        });
    }

    void UpdateCommand::Execute(const std::shared_ptr<Options>& options)
    {
        REQUIRE_PROJECT()
        IMPLEMENT_DEFAULT_OPTIONS();

        const std::filesystem::path modPath = "mods";
        if (!std::filesystem::exists(modPath))
            std::filesystem::create_directory(modPath);

        if (!options->name.empty())
        {
            project::mods::Mod::Update(options->name, options->force);
            return;
        }

        for (auto& file : utils::GetFilesByExtension(modPath, "pm"))
        {
            const auto modId = file.stem().string();
            project::mods::Mod::Update(modId, options->force);
        }
        spdlog::info("Updated all mods");
    }
}