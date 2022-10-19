#include <iostream>
#include <string>
#include <vector>
#include "progargs.h"

const std::string INSTRUCTIONS = "\timage in_path out_path oper\n\t\toperation: copy, histo, mono, gauss\n";

/**
 * Checks if the vector of strings (passed by reference) passes all rules
 */
unsigned short check_inputs(std::vector<std::string>& inputs){
	
	// The are 3 inputs
	if (inputs.size() != 3) {
		std::cout << std::endl << "Wrong format:" << std::endl << INSTRUCTIONS;
		return -1;
	// Operation is copy, histo, mono or gauss
	} else if( !(inputs[2] == "copy" || inputs[2] == "histo" || inputs[2] == "mono" || inputs[2] == "gauss")) {		
		std::cout << std::endl << "Unexpected operation:" << inputs[2] << std::endl << INSTRUCTIONS;
		return -1;
	}	
		
	// Input directory does not exist or cannot be opened
	
	// Output directory does not exist or cannot be opened
	
	return 0;
}
