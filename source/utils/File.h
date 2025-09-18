#pragma once
#include <fstream>
#include <stdexcept>

#include <curl/curl.h>
#include <spdlog/spdlog.h>

namespace provisioner::utils
{
    static size_t WriteData(void* ptr, const size_t size, const size_t nmemb, void* userdata)
    {
        auto* ofs = static_cast<std::ofstream*>(userdata);
        ofs->write(static_cast<char*>(ptr), size * nmemb);
        return size * nmemb;
    }

    static bool DownloadFile(const std::string& url, const std::filesystem::path& output_path)
    {
        CURL* curl = curl_easy_init();
        if (!curl)
        {
            throw std::runtime_error("Failed to initialize curl");
        }

        std::ofstream ofs(output_path, std::ios::binary);
        if (!ofs)
        {
            curl_easy_cleanup(curl);
            throw std::runtime_error("Failed to open file: " + output_path.string());
        }

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteData);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ofs);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, true);

        if (const CURLcode res = curl_easy_perform(curl); res != CURLE_OK)
        {
            ofs.close();
            curl_easy_cleanup(curl);
            throw std::runtime_error("curl_easy_perform() failed");
        }

        ofs.close();
        curl_easy_cleanup(curl);
        return true;
    }

    static size_t WriteString(void* contents, size_t size, size_t nmemb, std::string* s)
    {
        const size_t newLength = size * nmemb;
        s->append(static_cast<char*>(contents), newLength);
        return newLength;
    }

    static std::string FetchUrl(const std::string& url)
    {
        CURL* curl = curl_easy_init();
        if (!curl)
        {
            throw std::runtime_error("Failed to initialize curl");
        }

        std::string response;

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteString);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "lumentae/provisioner/dev");
        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        if (const CURLcode res = curl_easy_perform(curl); res != CURLE_OK)
        {
            curl_easy_cleanup(curl);
            throw std::runtime_error("curl_easy_perform() failed: " + std::string(curl_easy_strerror(res)));
        }

        long http_code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
        if (http_code != 200)
        {
            spdlog::critical("HTTP request failed: {}", http_code);
        }

        curl_easy_cleanup(curl);
        return response;
    }

    static std::string ReadFile(const std::filesystem::path& path)
    {
        std::ifstream file(path);
        if (!file.is_open())
        {
            throw std::runtime_error("Could not open file: " + path.string());
        }

        std::string content((std::istreambuf_iterator(file)), std::istreambuf_iterator<char>());
        file.close();
        return content;
    }

    static void WriteFile(const std::filesystem::path& path, const std::string& content)
    {
        std::ofstream file(path);
        if (!file.is_open())
        {
            throw std::runtime_error("Could not open file: " + path.string());
        }
        file << content;
        file.close();
    }

    static std::vector<std::filesystem::path> GetFilesByExtension(const std::filesystem::path& path,
                                                                  const std::string& filter = "",
                                                                  const bool recursive = false)
    {
        if (std::filesystem::is_regular_file(path))
            return {path};

        std::vector<std::filesystem::path> files;
        for (const auto& entry : std::filesystem::directory_iterator(path))
        {
            if (!entry.is_regular_file())
            {
                if (recursive && std::filesystem::is_directory(entry))
                    files.append_range(GetFilesByExtension(entry.path(), filter, recursive));
                else
                    continue;
            }

            if ((entry.path().extension().string().find(filter) != std::string::npos) && !
                std::filesystem::is_directory(entry))
                files.push_back(entry.path());
        }

        return files;
    }
}