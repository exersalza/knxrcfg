#include "config.hpp"


Config::Config(std::string&& path) : path(path) {
    config_paths["bare"] = std::vector<std::string> ();
    config_paths["named"] = std::vector<std::string> ();
}