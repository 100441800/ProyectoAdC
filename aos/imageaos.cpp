//
//  main.cpp
//  ProyectoAdC
//
//  Created by José David Rico Días on 17/10/22.
//

#include <iostream>
#include <vector>
#include <string>
#include <common/progargs.h>

int main(int argc, const char **argv) {
    // insert code here...
    std::cout << "AOS compilando!\n";
	std::vector<std::string> inputs(argv + 1, argv + argc);
	check_inputs(inputs);

    return 0;
}
