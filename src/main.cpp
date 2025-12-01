#include <array>
#include <cstddef>
#include <iostream>
#include <string>
#include <string_view>
#include <utility>

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
  size_t delimiter = commands.find(' ');
  std::string_view command = commands.substr(0, delimiter);
  std::string_view args = (delimiter != std::string_view::npos)
                              ? commands.substr(delimiter + 1)
                              : std::string_view{};
  function_handle_t func_handler = get_builtin(command);
  if (func_handler != NULL) {
    return func_handler(args);
  }
  return notfound(command);
}

// Built-in Functions
int echo_handler(std::string_view args) {
  std::cout << args;
  return 0;
}

int type_handler(std::string_view args) {
  size_t first_token_pos = args.find(" ");
  std::string_view first_arg = args.substr(0, first_token_pos);
  if (get_builtin(first_arg)) {
    std::cout << first_arg << " is a shell builtin";
  } else {
    std::cout << first_arg << ": not found";
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
