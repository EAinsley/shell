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
  // TODO: Shall we unified this part?
  if (func_handler != nullptr) {
    return func_handler(name_pair.first, args, ctx_);
  }
  std::optional<std::string> path = search_path(name_pair.first, ctx_.path_);
  if (path.has_value()) {
    return call_external_function(name_pair.first, args, path.value());
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
