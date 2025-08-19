#pragma once

#include <nlohmann/json.hpp>

namespace provisioner::project::mods
{
    struct ModUpdate
    {
        std::string platform = "";
        std::string id = "";
        std::string version = "";
    };

    struct ModDownload
    {
        std::string url = "";
        std::string sha512 = "";
    };

    struct ModData
    {
        std::string name = "Unnamed";
        std::string fileName = "Unnamed";
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(ModData, name, fileName)
};