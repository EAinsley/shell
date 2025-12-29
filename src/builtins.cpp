#include "builtins.h"

#include "shell_context.h"
#include "utils.h"
#include <filesystem>
#include <iostream>
#include <optional>
#include <string_view>
#include <unistd.h>

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

inline std::optional<std::string> search_path(std::string_view name,
                                              const ShellContext::Path &dirs) {
  namespace fs = std::filesystem;
  for (const auto &dir : dirs) {
    auto full_path = std::filesystem::path(dir) / name;
#ifdef _WIN32
    full_path += ".exe";
#endif
    // Watchout POSIX 0 means success
    if (fs::exists(full_path) &&
        (fs::is_regular_file(full_path) || fs::is_symlink(full_path)) &&
        access(full_path.c_str(), X_OK) == 0) {
      return full_path.string();
    }
  }
  return std::nullopt;
}

int type_handler(std::string_view args, const ShellContext &ctx) {
  std::string_view name = shell::tokenize_fist_sv(args).first;
  if (get_builtin(name)) {
    std::cout << name << " is a shell builtin";
  } else {
    auto path_to_name = search_path(name, ctx.path_);
    if (path_to_name.has_value()) {
      std::cout << name << " is " << path_to_name.value();
    } else {
      std::cout << name << ": not found";
    }
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
