#include "config.hpp"

#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

Config::Config(std::string &&path) : path(path) {
  config_paths["bare"] = std::vector<std::string>();
  config_paths["named"] = std::vector<std::string>();

  find_config();
}

int Config::find_config() {
  fs::path _path(".");

  for (auto &file : fs::recursive_directory_iterator(_path)) {
    auto file_name = file.path().filename();

    if (std::regex_match(file_name.c_str(), f_bare)){
      
      std::cout << file_name.string() << '\n';
    }
  }
  return 0;
}

int Config::load_config() {
  if (path.empty()) {
    if (find_config()) {
      std::cerr << "Can't find config file" << std::endl;
      return -1;
    }
  }

  return 0;
}