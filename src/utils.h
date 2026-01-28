#pragma once

#include "shell_context.h"
#include <filesystem>
#include <optional>
#include <ostream>
#include <string>
#include <string_view>
#include <vector>

namespace shell {
using ArgList = std::vector<std::string>;

std::vector<std::string_view> tokenize_sv(const std::string_view str_sv,
                                          char delimiter = ' ');
std::vector<std::string> tokenize(const std::string &str, char delimiter = ' ');
std::pair<std::string_view, std::optional<std::string_view>>
tokenize_fist_sv(const std::string_view str_sv, char delimeter = ' ');
std::pair<std::string, std::optional<std::string>>
tokenize_fist(const std::string &str, char delimiter = ' ');

std::optional<std::string> search_path(const std::string_view name,
                                       const ShellContext::Path &dirs);

std::ostream &operator<<(std::ostream &os, const ArgList &args);
} // namespace shell
// UTILS_H
