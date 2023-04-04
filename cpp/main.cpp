#include <iostream>
#include <fstream>

#include "config.hpp"

int main(int argc, char **argv) {
    Config config {"."};

    std::cout << config["name"] << std::endl;
    return 0;
}
