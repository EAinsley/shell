#pragma once
#include "shell_context.h"
#include "utils.h"
#include <array>
#include <string_view>

namespace shell {

using function_handle_t = int (*)(const std::string_view name,
                                  const ArgListSV &args,
                                  const ShellContext &context);

function_handle_t get_builtin(std::string_view name);

} // namespace shell
