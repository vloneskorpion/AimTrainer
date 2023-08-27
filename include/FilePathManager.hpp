#pragma once

#include "config.h"
#include "easylogging/easylogging++.h"

#include <filesystem>
#include <string>
#include <string_view>

class FilePathManager
{
    public:
        [[nodiscard]] static std::string getPath(const std::string_view fileName)
        {
            using namespace std::filesystem;
            for (const auto& entry : recursive_directory_iterator(rootPath))
            {
                if (is_regular_file(entry) and entry.path().filename() == fileName.data())
                {
                    return entry.path().string();
                }
            }

            LOG(ERROR) << "Couldn't find file: " << fileName;
            return "";
        }
};