#pragma once

#include <iostream>
#include <sstream>
#include <string>

namespace provisioner::utils
{
    template <typename T>
    static T Prompt(const std::string& question, T defaultValue)
    {
        std::string inputStr;
        T input;

        std::cout << question << " [" << defaultValue << "]: ";
        std::getline(std::cin, inputStr);
        if (inputStr.empty())
            return defaultValue;

        std::stringstream(inputStr) >> input;
        return input;
    }
}