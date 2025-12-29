#pragma once

#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace shell {
std::vector<std::string_view> tokenize_sv(const std::string_view str_sv,
                                          char delimiter = ' ');
std::vector<std::string> tokenize(const std::string &str, char delimiter = ' ');
std::pair<std::string_view, std::string_view>
tokenize_fist_sv(const std::string_view str_sv, char delimeter = ' ');
std::pair<std::string, std::string> tokenize_fist(const std::string &str,
                                                  char delimiter = ' ');
} // namespace shell
// UTILS_H
