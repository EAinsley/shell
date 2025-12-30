#include "builtins.h"

#include "shell_context.h"
#include "utils.h"
#include <iostream>
#include <optional>
#include <string_view>
#include <unistd.h>
#include <vector>

namespace shell {

namespace {

int echo_handler(const std::string_view /* unused */, const ArgListSV &args,
                 const ShellContext & /* unused */);
int type_handler(const std::string_view /* unused */, const ArgListSV &args,
                 const ShellContext &ctx);
int exit_handler(const std::string_view /* unused */,
                 const ArgListSV & /* unused */,
                 const ShellContext & /* unused */);

constexpr auto handler_map =
    std::array<std::pair<std::string_view, function_handle_t>, 3>{{
        {"echo", echo_handler},
        {"type", type_handler},
        {"exit", exit_handler},
    }};

// Built-in Functions
int echo_handler(const std::string_view name,
                 const std::vector<std::string_view> &args,
                 const ShellContext & /* unused */) {
  std::cout << args << std::endl;
  return 0;
}

int type_handler(const std::string_view /* unused */, const ArgListSV &args,
                 const ShellContext &ctx) {
  for (const std::string_view arg : args) {
    if (get_builtin(arg)) {
      std::cout << arg << " is a shell builtin\n";
    } else {
      auto path_to_name = search_path(arg, ctx.path_);
      if (path_to_name.has_value()) {
        std::cout << arg << " is " << path_to_name.value() << std::endl;
      } else {
        std::cout << arg << ": not found" << std::endl;
      }
    }
  }
  return 0;
}

int exit_handler(const std::string_view /* unused */,
                 const ArgListSV & /* unused */,
                 const ShellContext & /* unused */) {
  return 9;
}
} // namespace

function_handle_t get_builtin(std::string_view name) {
  for (auto [key, handler] : handler_map) {
    if (key == name)
      return handler;
  }
  return nullptr;
}

} // namespace shell
