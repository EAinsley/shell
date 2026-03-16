#pragma once

#include "shell_context.h"
#include <filesystem>
#include <optional>
#include <ostream>
#include <string>
#include <string_view>
#include <vector>

namespace shell {
using WordList = std::vector<std::string>;

std::vector<std::string> split(const std::string &str, char delimiter = ' ');

std::optional<std::string> search_path(const std::string_view name,
                                       const ShellContext::Path &dirs);

std::ostream &operator<<(std::ostream &os, const WordList &args);
} // namespace shell
// UTILS_H
