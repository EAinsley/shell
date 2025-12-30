#pragma once
#include "shell_context.h"
#include "utils.h"
#include <array>
#include <filesystem>
#include <string_view>

namespace shell {

using function_handle_t = int (*)(const std::string_view name,
                                  const ArgListSV &args,
                                  const ShellContext &context);

function_handle_t get_builtin(std::string_view name);

int call_external_function(const std::string_view name, const ArgListSV &args,
                           const std::string &path);

} // namespace shell
