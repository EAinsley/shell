#include <iostream>
#include <sstream>
#include <string>

bool echo(std::stringstream &commands) {
  std::cout << commands.str();
  return true;
}

bool notfound(const std::string command) {
  std::cout << command << ": command not found";
  return true;
}

bool eval(const std::string &commands) {
  std::stringstream command_ss(commands);
  std::string command;
  command_ss >> command;
  if (command == "exit")
    return false;
  if (command == "echo")
    return echo(command_ss);
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
    std::cout<<std::endl;
  }
  return 0;
}
