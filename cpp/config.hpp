#include <string>
#include <any>
#include <map>
#include <vector>


class Config {
public:
    explicit Config(std::string&& path = ".");
    
private:
    const std::string path;
    std::map<std::string, std::any> config;
    std::map<std::string, std::vector<std::string>> config_paths;
};