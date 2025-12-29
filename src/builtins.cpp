#include "builtins.h"

#include "shell_context.h"
#include "utils.h"
#include <iostream>
#include <string_view>

namespace shell {

namespace {

int echo_handler(std::string_view args, const ShellContext &_);
int type_handler(std::string_view args, const ShellContext &ctx);
int exit_handler(std::string_view _sv, const ShellContext &_sc);

constexpr auto handler_map =
    std::array<std::pair<std::string_view, function_handle_t>, 3>{{
        {"echo", echo_handler},
        {"type", type_handler},
        {"exit", exit_handler},
    }};

// Built-in Functions
int echo_handler(std::string_view args, const ShellContext &_) {
  std::cout << args;
  return 0;
}

int type_handler(std::string_view args, const ShellContext &ctx) {
  std::string_view name = shell::tokenize_fist_sv(args).first;
  if (get_builtin(name)) {
    std::cout << name << " is a shell builtin";
  } else {
    // Get path 
    std::cout << name << ": not found";
  }
  return 0;
}

int exit_handler(std::string_view _sv, const ShellContext &_sc) { return 9; }
} // namespace

function_handle_t get_builtin(std::string_view name) {
  for (auto [key, handler] : handler_map) {
    if (key == name)
      return handler;
  }
  return nullptr;
}

} // namespace shell
