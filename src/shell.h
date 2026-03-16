#pragma once
#include "command.h"
#include "shell_context.h"
#include "utils.h"
#include <array>
#include <string_view>
#include <vector>
namespace shell {

class Shell {
private:
  const ShellContext ctx_;
  unsigned int status_ = 0;
  int eval(const std::string &commands);
  void apply_redirectorys_(const std::vector<Redirector> &redirectors);

public:
  Shell();
  int run();
};

WordList parse_args(const std::string &str);
} // namespace shell
// SHELL_H
