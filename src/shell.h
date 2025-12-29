#pragma once
#include "shell_context.h"
#include <array>
#include <string_view>
#include <vector>
namespace shell {

class Shell {
private:
  const ShellContext ctx_;
  unsigned int status_ = 0;
  int eval(std::string_view commands);

public:
  Shell();
  int run();
};

} // namespace shell
// SHELL_H
