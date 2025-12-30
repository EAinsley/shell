#include "utils.h"
#include <string_view>
#include <unistd.h>
#include <vector>

namespace shell {
std::vector<std::string_view> tokenize_sv(const std::string_view str_sv,
                                          char delimiter) {
  std::vector<std::string_view> res;
  size_t delimiter_pos = 0, delimiter_prev = 0;
  while (delimiter_pos != std::string::npos) {
    delimiter_pos = str_sv.find(delimiter, delimiter_prev);
    res.emplace_back(
        str_sv.substr(delimiter_prev, delimiter_pos - delimiter_prev));
    delimiter_prev = delimiter_pos + 1;
  }
  return res;
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

std::optional<std::string> search_path(const std::string_view name,
                                       const ShellContext::Path &dirs) {
  namespace fs = std::filesystem;
  for (const auto &dir : dirs) {
    auto full_path = std::filesystem::path(dir) / name;
#ifdef _WIN32
    full_path += ".exe";
#endif
    // NOTE: POSIX 0 means success
    if (fs::exists(full_path) &&
        (fs::is_regular_file(full_path) || fs::is_symlink(full_path)) &&
        access(full_path.c_str(), X_OK) == 0) {
      return full_path.string();
    }
  }
  return std::nullopt;
}
std::ostream &operator<<(std::ostream &os, const ArgListSV &args) {
  if (args.empty()) {
    return os;
  }
  auto it = args.begin();
  for (; it + 1 != args.end(); ++it) {
    os << *it << ' ';
  }
  os << *it;
  return os;
}
} // namespace shell
