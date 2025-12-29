#pragma once
#include <array>
#include <string_view>

namespace shell {

class Shell {
private:
  unsigned int status_ = 0;
  int eval(std::string_view commands);

public:
  Shell() = default;
  int run();
};

} // namespace shell
// SHELL_H
