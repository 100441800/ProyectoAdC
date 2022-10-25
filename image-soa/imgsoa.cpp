#include <iostream>
#include <vector>
#include <string>
#include "common/progargs.hpp"

int main(int argc, const char **argv) {
	const std::vector<std::string> inputs(argv + 1, argv + argc);
	try {
        check_inputs(inputs);
        
    } catch (const std::exception& e) {
        std::cout << e.what();
    }
    return 0;
}
