#include "UpdateCommand.h"

#include "project/Project.h"
#include "project/mods/Mod.h"
#include "utils/File.h"

namespace provisioner::commands
{
    void UpdateCommand::Register(CLI::App* sub)
    {
        const auto opt = std::make_shared<Options>();

        sub->add_flag("-n,--name", opt->name);
        sub->callback([opt]()
        {
            Execute(opt);
        });
    }

    void UpdateCommand::Execute(const std::shared_ptr<Options>& options)
    {
        REQUIRE_PROJECT()

        const std::filesystem::path modPath = "mods";
        if (!std::filesystem::exists(modPath))
            std::filesystem::create_directory(modPath);

        if (!options->name.empty())
        {
            project::mods::Mod::Update(options->name);
            return;
        }

        for (auto& file : utils::GetFilesByExtension(modPath, "pm"))
        {
            const auto modId = file.stem().string();
            project::mods::Mod::Update(modId);
        }
        spdlog::info("Updated all mods");
    }
}
