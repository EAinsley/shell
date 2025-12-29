#pragma once
#include "shell_context.h"
#include <array>
#include <string_view>

namespace shell {

using function_handle_t = int (*)(std::string_view args,
                                  const ShellContext & context);

function_handle_t get_builtin(std::string_view name);

} // namespace shell
