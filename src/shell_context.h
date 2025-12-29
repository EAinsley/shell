#pragma once

#include <string>
#include <vector>

namespace shell {
class ShellContext {
public:
  using Path = std::vector<std::string>;
  Path path_;
  ShellContext(const char *path);
};
} // namespace shell
