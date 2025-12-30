#include "shell.h"

#include "builtins.h"
#include "utils.h"
#include <cstdlib>
#include <iostream>

namespace shell {

inline bool notfound(std::string_view command) {
  std::cout << command << ": command not found\n";
  return true;
}

Shell::Shell() : ctx_(std::getenv("PATH")) {}

int Shell::eval(std::string_view commands) {
  auto name_pair = tokenize_fist_sv(commands);
  auto args = tokenize_sv(name_pair.second);
  function_handle_t func_handler = get_builtin(name_pair.first);
  if (func_handler != nullptr) {
    return func_handler(name_pair.first, args, ctx_);
  }
  return notfound(name_pair.first);
}

int Shell::run() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
  std::string command_line;
  while (true) {
    std::cout << "$ ";
    std::getline(std::cin, command_line);
    if (!command_line.empty()) {
      status_ = eval(command_line);
      if (status_ == 9)
        return 0;
    }
  }
  return 0;
}

} // namespace shell
