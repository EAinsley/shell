#include "shell_context.h"

#include "utils.h"
namespace shell {

ShellContext::ShellContext(const char *path) {
  if (path) {
#ifdef _WIN32
    path_ = tokenize(path, ';');
#else
    path_ = tokenize(path, ':');
#endif
  }
}
} // namespace shell
