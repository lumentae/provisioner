#pragma once
#include <algorithm>
#include <string>

namespace provisioner::utils
{
    inline std::string ToLower(const std::string& str)
    {
        std::string result = str;
        std::ranges::transform(result, result.begin(), ::tolower);
        return result;
    }
}