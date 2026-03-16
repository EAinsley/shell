#include "shell.h"

#include "builtins.h"
#include "utils.h"
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <utility>

#include "command.h"

namespace shell {

inline bool notfound(std::string_view command) {
  std::cout << command << ": command not found\n";
  return true;
}

Shell::Shell() : ctx_(std::getenv("PATH")) {}

int Shell::eval(const std::string &commands) {
  // It should not be empty string.
  assert(!commands.empty());

  AbstractCommand ac = parser::parse(commands);

  function_handle_t func_handler = get_builtin(ac.command);
  // TODO: Shall we unified this part?
  if (func_handler != nullptr) {
    return func_handler(ac.command, ac.arguments, ctx_);
  }
  std::optional<std::string> path = search_path(ac.command, ctx_.path_);
  if (path.has_value()) {
    return call_external_function(ac.command, ac.arguments, path.value());
  }
  return notfound(ac.command);
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

WordList parse_args(const std::string &str) {
  parser::Lexer lex(str);
  return lex.words_str();
}

} // namespace shell
