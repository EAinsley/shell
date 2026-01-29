#include "shell_context.h"

#include "utils.h"
namespace shell {

ShellContext::ShellContext(const char *path) {
  if (path) {
#ifdef _WIN32
    path_ = split(path, ';');
#else
    path_ = split(path, ':');
#endif
  }
}
} // namespace shell
