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

    inline std::string ToUpper(const std::string& str)
    {
        std::string result = str;
        std::ranges::transform(result, result.begin(), ::toupper);
        return result;
    }

    template <typename T>
    std::string Join(const T& items, const std::string& delim)
    {
        std::ostringstream s;
        for (const auto& i : items)
        {
            if (&i != &items[0])
            {
                s << delim;
            }
            s << i;
        }
        return s.str();
    }
}