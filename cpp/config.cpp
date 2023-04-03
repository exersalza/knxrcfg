#include "config.hpp"

#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>

namespace fs = std::filesystem;


Config::Config(std::string &&path) : path(path) {
    find_config();
    load_config();
}

int Config::find_config() {
    fs::path _path(path);

    if (std::regex_match(path, f_bare)) {
        config_path = path;
        return 0;
    }

    for (auto &file: fs::recursive_directory_iterator(_path)) {
        auto file_name = file.path().filename();

        if (std::regex_match(file_name.c_str(), f_bare)) {

            config_path = file_name.string();
            return 0;
        }
    }

    std::cerr << "Couldn't find config file.\n";
    return 1;
}

int Config::load_config() {
    std::ifstream file(config_path);

    if (!file.is_open()) {
        std::cerr << "Can't open " << config_path << "!\n";
    }

    std::string line;
    std::string last_category;

    while (std::getline(file, line)) {
        // sort out empty and comment lines.
        if (line.empty()) {
            last_category = "";
            continue;
        }
        if (line[0] == '/' && line[1] == '/') continue;

        // get category and lists
        if (std::regex_match(line, l_category)) {
            line.replace(line.find('('), 1, "");
            line.replace(line.find(')'), 1, "");

            last_category = line;
            continue;
        }

        // ^ fun stuff, now it's getting jazzy


        if (std::regex_match(line, l_list)) {

        }
    }

    file.close();
    return 0;
}
