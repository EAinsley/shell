#pragma once
#include "utils.h"
#include <algorithm>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>
/*
 * In this package we have ShellCommandStream, which is a small wrapper of the
 * Command that is easy to peek and get the next.
 * NOTE: This might be used for later if we want to make take the stream
 * continously.
 *
 * Then we have Lexer class, which is responsible for lexing the code. Now, it
 * won't be too useful to keep a state, but it might be useful in the future
 * if we want to make it lazy or on-time lexing. It will holds the
 * AbstractCommand.
 * TODO:Parser
 * We haven't decided how to write the parser. Maybe we shouldn't write a class,
 * but only a function. It will make the words into a AbstractCommand
 *
 * The AbstractCommand should be the struct that shell actually execute. It
 * will contain command, args, a way to get cstyle arglist and include necessary
 * environment such as pipes and file discripters.
 * * */

namespace shell {

enum class RedirectorType { Write, Append };

struct Redirector {
  int target_fd;
  RedirectorType type;
  std::string filename;
};

struct AbstractCommand {
  std::string command;
  WordList arguments;
  std::vector<Redirector> redirectors;
};

namespace parser {
enum class WordType { Text, Stdin, Stderr, Eof };

struct Word {
  WordType type;
  std::optional<std::string> text;
};
/* Provide a command stream that have next, peek and eof functionality.
 * TODO: Maybe in the future, we should be able to add more commands to the
 * stream, so that it could have multi-line functions. I actually don't know how
 * should this be done properly....
 */
class ShellCommandStream {
private:
  std::string command_;
  size_t current_pos_ = -1;

public:
  ShellCommandStream() = default;
  explicit ShellCommandStream(std::string_view command) : command_(command) {};

  ShellCommandStream(const ShellCommandStream &) = delete;
  ShellCommandStream &operator=(ShellCommandStream &) = delete;

  char next();
  char peek() const;
  char now() const;
  bool eof() const;
};

class Lexer {
private:
  ShellCommandStream shell_command_stream_;
  std::vector<Word> parsed_words_;

  std::string consume_single_quote_();
  std::string consume_double_quote_();
  void consume_normal_();
  // void consume_all_();

  // TODO: Do we need to separate LexerStream and Lexer?
  int pos_ = -1;

public:
  Lexer() = delete;
  explicit Lexer(std::string_view command); // We will do cosume_ here.

  const std::vector<Word> &words() const;
  const WordList words_str() const;

  const WordType next();
  const WordType peek() const;
  const WordType now() const;
  const std::optional<std::string> word() const;
  bool eof() const;
};
// class Parser {
// private:
//   AbstractCommand parsed_command_;
//
// public:
//   explicit Parser(std::string_view command);
//   const AbstractCommand &abstract_command() const;
// };
//

AbstractCommand parse(std::string_view command);

} // namespace parser
} // namespace shell
