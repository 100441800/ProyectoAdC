#include <filesystem>
#include <stdexcept>
#include <string>

#include "progargs.hpp"


namespace fs = std::filesystem;

const std::string INSTRUCTIONS =
    "\timage in_path out_path oper\n\t\toperation: copy, histo, mono, gauss\n";

/**
 * Checks if the vector of strings (passed by reference) passes all rules
 */
void check_inputs(const std::vector<std::string> &inputs) {
  if (inputs.size() != 3) {
    throw std::invalid_argument("Wrong format:\n" + INSTRUCTIONS);
  } else if (!(inputs[2] == "copy" || inputs[2] == "histo" ||
               inputs[2] == "mono" || inputs[2] == "gauss")) {
    throw std::invalid_argument("Unexpected operation:" + inputs[2] + "\n" +
                                INSTRUCTIONS);
  }
  // Directories exists
  const fs::path in_path = inputs[0];
  const fs::path out_path = inputs[1];
  if (!fs::exists(in_path)) {
    throw std::invalid_argument("Input directory [" + inputs[0] +
                                "] does not exist\n" + INSTRUCTIONS);
  } else if (!fs::exists(out_path)) {
    throw std::invalid_argument("Output directory [" + inputs[1] +
                                "] does not exist\n" + INSTRUCTIONS);
  }
  // Read and write permissions
  if ((fs::status(inputs[0]).permissions() & fs::perms::owner_read) ==
      fs::perms::none) {
    throw std::invalid_argument("Cannot open directory [" + inputs[0] + "]\n" +
                                INSTRUCTIONS);
  } else if ((fs::status(inputs[1]).permissions() & fs::perms::owner_write) ==
             fs::perms::none) {
    throw std::invalid_argument("Cannot open directory [" + inputs[1] + "]\n" +
                                INSTRUCTIONS);
  }
}
