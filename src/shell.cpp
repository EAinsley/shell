#include "shell.h"

#include "builtins.h"
#include "utils.h"
#include <cstdlib>
#include <iostream>
#include <utility>

namespace shell {

inline bool notfound(std::string_view command) {
  std::cout << command << ": command not found\n";
  return true;
}

Shell::Shell() : ctx_(std::getenv("PATH")) {}

int Shell::eval(const std::string &commands) {
  auto name_pair = tokenize_fist(commands);
  ArgList args;
  if (name_pair.second.has_value()) {
    args = parse_args(name_pair.second.value());
  }
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

ArgList parse_args(const std::string &str) {
  bool is_quoting = false;
  std::string arg;
  std::vector<std::string> args;
  arg.reserve(16);
  for (char c : str) {
    if (c == '\'') {
      is_quoting = !is_quoting;
    } else if (c == ' ') {
      if (is_quoting) {
        arg.push_back(' ');
      } else if (!arg.empty()) {
        args.push_back(arg);
        arg.clear();
      }
      // Otherwise, it's consecutive space...ignoring
    } else {
      arg.push_back(c);
    }
  }
  if (!arg.empty()) {
    args.push_back(arg);
  }
  return args;
}

} // namespace shell
