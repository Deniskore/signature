#include <iostream>

#include "hashcalc.h"
#include "utils.h"

int main(int argc, char* argv[]) {
  std::cout << "Signature tool v 1.0" << std::endl;
  std::cout << "Thread count: " << get_threads_count() << std::endl;
  std::pair<std::string, std::string> files;
  auto block_size = HashCalc::one_megabyte;

  if (argc != 4) {
    std::cout << "Wrong number of arguments" << std::endl;
    std::cout << "Should be like this: signature test_file1.bin "
                 "test_file_hash.txt 1048576"
              << std::endl;
    std::exit(0);
  }

  if (argv[1]) {
    if (!fs::is_regular_file(argv[1])) {
      std::cout << "Input file is not valid" << std::endl;
      std::exit(0);
    }
    files.first = argv[1];
  }

  if (argv[2]) {
    files.second = argv[2];
  }

  const auto arg_block_size = std::strtoll(argv[3], nullptr, 10);
  if (arg_block_size != 0 && arg_block_size != LLONG_MAX && arg_block_size != LLONG_MIN) {
    block_size = uint64_t(arg_block_size);
  }

  CHashCalc calc(files.first, files.second, block_size);
  calc.run();
}
