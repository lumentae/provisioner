#include "ModifyCommand.h"

#include <complex>

#include "project/Project.h"
#include "project/mods/Mod.h"
#include "utils/Prompt.h"

namespace provisioner::commands
{
    void ModifyCommand::Register(CLI::App* sub)
    {
        const auto opt = std::make_shared<Options>();

        sub->callback([opt]()
        {
            Execute(opt);
        });
    }

    void ModifyCommand::Execute(const std::shared_ptr<Options>& options)
    {
        REQUIRE_PROJECT()

    start:

        spdlog::info("Modifying {}", project.mData.name);
        spdlog::info("(n)  name = {}", project.mData.name);
        spdlog::info("(v)  version = {}", project.mData.version);
        spdlog::info("minecraft = {{");
        spdlog::info("(m.v) version = {}", project.mData.minecraft.version);
        spdlog::info("(m.t) type = {}", project.mData.minecraft.type);
        spdlog::info("(m.l) loaderVersion = {}", project.mData.minecraft.loaderVersion);
        spdlog::info("}}");
        spdlog::info("(av) allowedVersions = [{}]", utils::Join(project.mData.allowedVersions, ", "));

        auto response = utils::Prompt<std::string>(
            "What field should be modified? (e.g. minecraft.version, 'quit' to quit)", "");
        if (response.empty())
            return;

        response = utils::ToLower(response);
        if (response == "name" || response == "n")
        {
            project.mData.name = utils::Prompt<std::string>("New name", project.mData.name);
        }
        else if (response == "version" || response == "v")
        {
            project.mData.version = utils::Prompt<std::string>("New version", project.mData.version);
        }
        else if (response == "minecraft.version" || response == "m.v")
        {
            project.mData.minecraft.version = utils::Prompt<std::string>(
                "New Minecraft version", project.mData.minecraft.version);
        }
        else if (response == "minecraft.type" || response == "m.t")
        {
            project.mData.minecraft.type = utils::Prompt<std::string>("New loader (fabric, neoforge, vanilla)",
                                                                      project.mData.minecraft.type);
        }
        else if (response == "minecraft.loaderversion" || response == "m.l")
        {
            project.mData.minecraft.loaderVersion = utils::Prompt<std::string>(
                "New loader version", project.mData.minecraft.loaderVersion);
        }
        else if (response == "allowedversions" || response == "av")
        {
        }
        else if (response == "quit" || response == "q" || response == "exit" || response == "e" || response == "stop")
        {
            project.Save();
            return;
        }
        else
        {
            spdlog::error("Invalid field");
            return;
        }

        project.Save();
        goto start;
    }
}