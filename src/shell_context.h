#pragma once

#include <string>
#include <unistd.h>
#include <vector>

namespace shell {
class ShellContext {
public:
  using Path = std::vector<std::string>;
  Path path_;
  ShellContext(const char *path);
};

class FDSaver {
private:
  int in = 0, out = 1, err = 2;

public:
  FDSaver() {
    in = dup(0);
    out = dup(1);
    err = dup(2);
  }
  ~FDSaver() {
    dup2(in, 0);
    dup2(out, 1);
    dup2(err, 2);
    close(in);
    close(out);
    close(err);
  }
};
} // namespace shell
