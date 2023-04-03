#pragma once

#include <any>
#include <map>
#include <string>
#include <vector>
#include <regex>

class Config {
public:
    explicit Config(std::string &&path);

private:
    const std::string path;
    std::map <std::string, std::any> config;
    std::string config_path;

    int load_config();

    int find_config();

    std::regex f_bare = std::regex("(\\w+)?((\\.knxrcfg$)|(\\.kxcfg$))");
    std::regex l_category = std::regex("(\\(.+\\))");
    std::regex l_list = std::regex("(\\[.+\\])");
};