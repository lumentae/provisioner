#pragma once
#include <optional>
#include <nlohmann/json.hpp>

NLOHMANN_JSON_NAMESPACE_BEGIN

namespace detail
{
    template <typename BasicJsonType, typename T>
    void from_json(const BasicJsonType& j, std::optional<T>& opt)
    {
        if (j.is_null())
        {
            opt = std::nullopt;
        }
        else
        {
            opt.emplace(j.template get<T>());
        }
    }
}

}
}