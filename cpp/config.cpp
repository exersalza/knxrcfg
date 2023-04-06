#include "config.hpp"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <limits>
#include <any>

namespace fs = std::filesystem;

void move_stuff(const std::string& yep, std::vector<any_nested>& where_to_move);

struct parsed_line {
    std::string key;
    std::string value;
};

template <typename T>
bool is_numeric(const std::string& str) {
    std::istringstream iss(str);
    T num;

    if (!(iss >> num) || (!iss.eof() && iss.peek() != '\n')) return false;

    return (num >= std::numeric_limits<T>::min() && num <= std::numeric_limits<T>::max());
}

std::vector<std::string> split(const std::string& str, const std::string& del) {
    std::regex reg(del);
    std::sregex_token_iterator iter(str.begin(), str.end(), reg, -1);
    std::vector<std::string> ret(iter, std::sregex_token_iterator());


    return ret;
}

std::string strip(const std::string& str) {
    return std::regex_replace(str, std::regex("^\\s+|\\s+$"), "");
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
        if (line.empty() || line.length() < 2) {
            last_category = "";
            continue;
        }

        if (std::regex_match(line, std::regex("^//.+|^/.+"))) {
            std::cout << "yeet\n";
            continue;
        }


        line = std::regex_replace(line, std::regex("//.+$"), "");

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
        parsed_line kv {strip(split_line[0]), strip(split_line[1])};
          
        if (std::regex_match(kv.value, l_is_string)) {
            kv.value = std::regex_replace(kv.value, std::regex("[\"']"), "");
        }

        if (std::regex_match(kv.value, l_list)) {
            kv.value = std::regex_replace(kv.value, std::regex("[\\[\\]]"), "");
            std::vector<std::string> t = split(kv.value, ",");
            std::vector<any_nested> ret;
            ret.reserve(t.size());

            for (std::string& i : t) {
                std::string yep = strip(i);

                move_stuff(yep, ret);
            }

//            if (!last_category.empty()){
//                if (!config.count(last_category)) {
//                    config[last_category] = std::map<std::string, _any> {};
//                }
//
//
//            }
        }
    }

    file.close();
    return 0;
}

void move_stuff(const std::string& yep, std::vector<any_nested>& where_to_move ) {
    // maybe not the optimal way, but it works, maybe I fix it later.
    if (is_numeric<int>(yep)) {
        where_to_move.push_back(std::stoi(yep));
        return;
    }

    if (is_numeric<double>(yep)) {
        where_to_move.push_back(std::stod(yep));
        return;
    }

    where_to_move.push_back(yep);
}
