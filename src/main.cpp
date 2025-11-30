#include <cstddef>
#include <iostream>
#include <string>
#include <string_view>

bool echo(const std::string_view &args) {
  std::cout << args;
  return true;
}

bool notfound(const std::string_view command) {
  std::cout << command << ": command not found";
  return true;
}

bool eval(const std::string_view &commands) {
  size_t delimiter = commands.find(' ');
  std::string_view command = commands.substr(0, delimiter);
  std::string_view args = (delimiter != std::string_view::npos)
                              ? commands.substr(delimiter + 1)
                              : std::string_view{};
  if (command == "exit")
    return false;
  if (command == "echo")
    return echo(args);
  return notfound(command);
}

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
  std::string command_line;
  while (true) {
    std::cout << "$ ";
    std::getline(std::cin, command_line);
    if (!eval(command_line))
      break;
    std::cout << std::endl;
  }
  return 0;
}
