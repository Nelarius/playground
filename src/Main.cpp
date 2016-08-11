#include "app/Application.h"
#include "Wren++.h"
#include <algorithm>
#include <cstdio>
#include <cstdlib>  // for std::exit()
#include <string>
#include <vector>

void parseArguments(const std::vector<std::string>& arguments, std::string& wrenTest) {
    for (auto it = arguments.begin(); it != arguments.end(); it += 2) {
        if (*it == "--test") {
            wrenTest = *(it + 1);
        }
        else {
            std::printf("Unknown option: %s\n", it->c_str());
            std::exit(EXIT_SUCCESS);
        }
    }
}

void printHelp() {
    std::printf("Usage: The playground engine.\n");
    std::printf("--help : this help.\n");
    std::printf("--test [wren file] : execute a wren test suite.\n");
}

int main( int argc, char** argv ) {
    // we skip the first argument, because it is the name of the program
    std::vector<std::string> arguments(argv + 1, argv + argc);
    if (argc == 2) {
        if (std::find(arguments.begin(), arguments.end(), "--help") != arguments.end()) {
            printHelp();
        }
    }
    else if (argc > 2) {
        std::string wrenTest;
        parseArguments(arguments, wrenTest);

        wrenpp::VM vm;
        vm.executeModule(wrenTest);
    }
    else {
        pg::Application app{};
        app.run();
    }

    return 0;
}
