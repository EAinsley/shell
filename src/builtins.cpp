#include "builtins.h"

#include "shell_context.h"
#include "utils.h"
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <optional>
#include <string_view>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

namespace shell {

namespace {

// TODO: I really need to change this registry part... Not sure if this can be
// acheived by some meta programming techniques. Or we could just register them
// at runtime and lazily...
int echo_handler(const std::string_view /* unused */, const ArgList &args,
                 const ShellContext & /* unused */);
int type_handler(const std::string_view /* unused */, const ArgList &args,
                 const ShellContext &ctx);
int exit_handler(const std::string_view /* unused */,
                 const ArgList & /* unused */,
                 const ShellContext & /* unused */);
int pwd_handler(const std::string_view /* unused */,
                const ArgList & /* unused */,
                const ShellContext & /* unused */);
int cd_handler(const std::string_view /* unused */, const ArgList &args,
               const ShellContext & /* unused */);

constexpr auto handler_map =
    std::array<std::pair<std::string_view, function_handle_t>, 5>{
        {{"echo", echo_handler},
         {"type", type_handler},
         {"exit", exit_handler},
         {"pwd", pwd_handler},
         {"cd", cd_handler}}};

// Built-in Functions
int echo_handler(const std::string_view name_sv, const ArgList &args,
                 const ShellContext & /* unused */) {
  std::cout << args << std::endl;
  return 0;
}

int type_handler(const std::string_view /* unused */, const ArgList &args,
                 const ShellContext &ctx) {
  for (const std::string& arg : args) {
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
                 const ArgList & /* unused */,
                 const ShellContext & /* unused */) {
  return 9;
}

int pwd_handler(const std::string_view /* unused */,
                const ArgList & /* unused */,
                const ShellContext & /* unused */) {
  std::cout << std::filesystem::current_path().string() << std::endl;
  return 0;
}

int cd_handler(const std::string_view /* unused */, const ArgList &args,
               const ShellContext & /* unused */) {
  if (args.size() > 1) {
    std::cerr << "Too many args for cd command\n";
  }
  std::filesystem::path to_path = (args.empty() || args[0] == "~")
                                      ? getenv("HOME")
                                      : args[0];
  try {
    std::filesystem::current_path(to_path);
  } catch (std::filesystem::filesystem_error const &ex) {
    std::cerr << "cd: " << args[0] << ": " << ex.code().message() << '\n';
    return ex.code().value();
  }
  return 0;
}
} // namespace

function_handle_t get_builtin(std::string_view name) {
  for (auto [key, handler] : handler_map) {
    if (key == name)
      return handler;
  }
  return nullptr;
}

int call_external_function(const std::string_view name_sv,
                           const ArgList &args, const std::string &path) {
  pid_t pid = fork();
  if (pid < 0) {
    std::cerr << "Fork Error.\n";
    return -1;
  }
  if (pid == 0) {
    // Copy on demand, only for the child.
    std::string name(name_sv);
    // ISSUE: Wait...why they cannot be <const char *> ...
    std::vector<char *> argv;
    argv.emplace_back(const_cast<char *>(name.data()));
    for (const auto &arg : args) {
      argv.emplace_back(const_cast<char *>(arg.data()));
    }
    argv.emplace_back(nullptr);
    execv(path.data(), argv.data());
    std::cerr << "execv error.\n";
    _exit(1);
  } else {
    int status;
    waitpid(pid, &status, 0);
    if (WIFEXITED(status))
      return WEXITSTATUS(status);
    else
      return -1;
  }
}
} // namespace shell
