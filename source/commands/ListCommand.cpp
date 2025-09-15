#include "ListCommand.h"

#include "project/Project.h"
#include "project/mods/Mod.h"
#include "utils/File.h"

namespace provisioner::commands
{
    void ListCommand::Register(CLI::App* sub)
    {
        const auto opt = std::make_shared<Options>();

        sub->callback([opt]()
        {
            Execute(opt);
        });
    }

    void ListCommand::Execute(const std::shared_ptr<Options>& options)
    {
        REQUIRE_PROJECT()

        const std::filesystem::path modPath = "mods";
        if (!std::filesystem::exists(modPath))
            std::filesystem::create_directory(modPath);

        spdlog::info("Installed mods for {}", project.mData.name);
        for (auto& file : utils::GetFilesByExtension(modPath, "pm"))
        {
            const project::mods::ModData modData = nlohmann::json::parse(utils::ReadFile(file));

            spdlog::info("- {} [{}]", modData.name, modData.slug);
            spdlog::info("  Version: {}", modData.update.version);
            spdlog::info("  Platform: {}", modData.platform);
            spdlog::info("");
        }
    }
}