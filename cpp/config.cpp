#include "config.hpp"

#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>

namespace fs = std::filesystem;

struct parsed_line {
    std::string key;
    std::string value;
};

std::vector<std::string> split(const std::string& str, const std::string& del) {
    std::regex reg(del);
    std::sregex_token_iterator iter(str.begin(), str.end(), reg, -1);
    std::vector<std::string> ret(iter, std::sregex_token_iterator());


    return ret;
}


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
        return -1;
    }

    std::string line;
    std::string last_category;

    while (std::getline(file, line)) {
        // sort out empty and comment lines.
        if (line.empty() || line.length() < 2 || (line[0] == '/' && line[1] == ' ')) {
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
        std::vector<std::string> split_line = split(line, "=");

        if (split_line.size() < 2) continue; // I don't know how you can fuck up the config.
        parsed_line kv {split_line[0], split_line[1]};
          
        if (std::regex_match(kv.value, l_is_string)) {
            kv.value = std::regex_replace(kv.value, std::regex("[\"']"), "");
        }

        if (std::regex_match(kv.value, l_list)) {
            kv.value = std::regex_replace(kv.value, std::regex("[\\[\\]]"), "");
            std::vector<std::string> t = split(kv.value, ", ");

            for (std::string& i : t) {
                std::cout << i << '\n';
            }
        }
    }

    file.close();
    return 0;
}
