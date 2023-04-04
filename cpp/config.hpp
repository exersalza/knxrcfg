#pragma once

#include <any>
#include <map>
#include <string>
#include <vector>
#include <regex>
#include <iostream>

class Config {
public:
    explicit Config(std::string &&path);
    
    std::any& operator[](const std::string& key) { return config[key]; };

    template<typename T>
    friend std::ostream& operator<<(std::ostream& os, const T& value) {
        os << value;
        return os;
    };

    friend std::ostream& operator<<(std::ostream& os, const Config& map) { return os; };
private:
    const std::string path;
    std::map <std::string, std::any> config;
    std::string config_path;

    int load_config();

    int find_config();

    std::regex f_bare = std::regex("(\\w+)?((\\.knxrcfg$)|(\\.kxcfg$))");
    std::regex l_category = std::regex("(\\(.+\\))");
    std::regex l_list = std::regex("(\\[.+\\])");

    std::regex l_is_string = std::regex("([\"'].*[\"'])");
};


inline std::ostream& operator<<(std::ostream& os, const std::any& value) {
    if (value.type() == typeid(int)) {
        os << std::any_cast<int>(value);
    }

    if (value.type() == typeid(double)) {
        os << std::any_cast<double>(value);
    }

    if (value.type() == typeid(std::string)) {
        os << std::any_cast<std::string>(value);
    }

    return os;
};
