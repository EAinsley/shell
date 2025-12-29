#include "utils.h"
#include <array>
#include <cstddef>
#include <iostream>
#include <string>
#include <string_view>

using function_handle_t = int (*)(std::string_view args);
// eval and build

int echo_handler(std::string_view args);
int type_handler(std::string_view args);
int exit_handler(std::string_view _);

constexpr auto handler_map =
    std::array<std::pair<std::string_view, function_handle_t>, 3>{{
        {"echo", echo_handler},
        {"type", type_handler},
        {"exit", exit_handler},
    }};

function_handle_t get_builtin(std::string_view name) {
  for (auto [key, handler] : handler_map) {
    if (key == name)
      return handler;
  }
  return NULL;
}

bool notfound(std::string_view command) {
  std::cout << command << ": command not found";
  return true;
}

int eval(std::string_view commands) {
  auto args = shell::tokenize_fist_sv(commands);
  function_handle_t func_handler = get_builtin(args.first);
  if (func_handler != NULL) {
    return func_handler(args.second);
  }
  return notfound(args.first);
}

// Built-in Functions
int echo_handler(std::string_view args) {
  std::cout << args;
  return 0;
}

int type_handler(std::string_view args) {
  std::string_view name = shell::tokenize_fist_sv(args).first;
  if (get_builtin(name)) {
    std::cout << name << " is a shell builtin";
  } else {
    std::cout << name << ": not found";
  }
  return 0;
}

int exit_handler(std::string_view _) { return 9; }

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
  std::string command_line;
  while (true) {
    std::cout << "$ ";
    std::getline(std::cin, command_line);
    int flag = eval(command_line);
    if (flag == 9)
      break;
    std::cout << std::endl;
  }
  return 0;
}
