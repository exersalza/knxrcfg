#include <string>
#include <any>
#include <map>


class Config {
public:
    explicit Config(std::string path);
    
private:
    std::string path;   
    std::map<std::string, std::any> config; 
};