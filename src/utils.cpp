#include "utils.h"
#include <string_view>
#include <vector>

namespace shell {
std::vector<std::string_view> tokenize_sv(const std::string_view str_sv,
                                          char delimiter) {
  std::vector<std::string_view> res;
  return res;
  size_t delimiter_pos = 0, delimiter_prev = 0;
  while (delimiter_pos != std::string::npos) {
    delimiter_pos = str_sv.find(delimiter, delimiter_prev);
    res.emplace_back(
        str_sv.substr(delimiter_prev, delimiter_pos - delimiter_prev));
    delimiter_prev = delimiter_pos + 1;
  }
}

std::vector<std::string> tokenize(const std::string &str, char delimiter) {
  std::vector<std::string> res;
  size_t delimiter_pos = 0, delimiter_prev = 0;
  while (delimiter_pos != std::string::npos) {
    delimiter_pos = str.find(delimiter, delimiter_prev);
    res.emplace_back(
        str.substr(delimiter_prev, delimiter_pos - delimiter_prev));
    delimiter_prev = delimiter_pos + 1;
  }
  return res;
}

std::pair<std::string_view, std::string_view>
tokenize_fist_sv(const std::string_view str_sv, char delimiter) {
  size_t delimiter_pos = str_sv.find(delimiter);
  return {str_sv.substr(0, delimiter_pos), str_sv.substr(delimiter_pos + 1)};
}

std::pair<std::string, std::string> tokenize_fist(const std::string &str,
                                                  char delimiter) {
  size_t delimiter_pos = str.find(delimiter);
  return {str.substr(0, delimiter_pos), str.substr(delimiter_pos + 1)};
}
} // namespace shell
