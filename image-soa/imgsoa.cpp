#include "common/progargs.hpp"
#include "soa/imagesoa.hpp"
#include <chrono>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

int main(int argc, const char **argv) {
  const std::vector<std::string> inputs(argv + 1, argv + argc);
  try {
    check_inputs(inputs);
    const std::filesystem::path in_dir{inputs[0]};
    const std::filesystem::path out_dir{inputs[1]};
    std::cout << "Input path: " << inputs[0] << std::endl
              << "Output path: " << inputs[1] << std::endl;
    for (auto const &dir_entry : std::filesystem::directory_iterator{in_dir}) {
      ImageSoa image(dir_entry.path());
      if (inputs[2] == "copy")
        image.copy(out_dir);
      else if (inputs[2] == "histo")
        image.histo(out_dir);
      else if (inputs[2] == "mono")
        image.mono(out_dir);
      else if (inputs[2] == "gauss")
        image.gauss(out_dir);
      std::cout << "File: " << dir_entry.path() << "(time: "
                << image.load_time + image.operation_time + image.store_time
                << ")" << std::endl
                << "\tLoad time: " << image.load_time << std::endl;
      std::cout << "\t" << inputs[2] << " time: " << image.operation_time
                << std::endl
                << "\tStore time: " << image.store_time << std::endl;
    }
  } catch (const std::exception &e) {
    std::cout << e.what() << std::endl;
  }
  return 0;
}