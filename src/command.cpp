#include "command.h"
#include <stdexcept>
#include <vector>

namespace shell {
namespace parser {

char ShellCommandStream::next() {
  if (eof()) {
    throw std::runtime_error("[SelleCommandStream] consume eof.\n");
  }
  return command_[++current_pos_];
}

char ShellCommandStream::peek() {
  if (current_pos_ + 1 < command_.size()) {
    return command_[current_pos_ + 1];
  }
  return '\0';
}

bool ShellCommandStream::eof() { return current_pos_ + 1 >= command_.size(); }

Lexer::Lexer(std::string_view command) : shell_command_stream_(command) {
  consume_normal_();
}

const WordList Lexer::words_str() const {
  WordList res;
  for (const auto &word : this->parsed_words_) {
    res.emplace_back(word.text.value());
  }
  return res;
}

const std::vector<Word> &Lexer::words() const { return parsed_words_; }
std::string Lexer::consume_single_quote_() {
  std::string text;
  text.reserve(16);

  try {
    while (true) {
      char c = shell_command_stream_.next();
      if (c == '\'') {
        if (shell_command_stream_.peek() == '\'') {
          shell_command_stream_.next();
          continue;
        }
        return text;
      }
      if (c == '\\') {
        text.push_back(shell_command_stream_.next());
        continue;
      }
      text.push_back(c);
    }
  } catch (const std::runtime_error &e) {
    // QUESTION: How to add new information to error?
    // FIX:Merge two error.
    throw std::runtime_error("[Lexer] Single quote ended unexpectedly\n");
    throw e;
  }
}
/* NOTE: Requirements from codecrafter:
 * Within double quotes, a backslash only escapes certain special characters: ",
 * \, $, `, and newline. For all other characters, the backslash is treated
 * literally.
 * In this stage, we’ll cover:
 *     \": escapes double quote, allowing " to appear literally within the
 * quoted string.
 *     \\: escapes backslash, resulting in a literal \.
 * We won’t cover the following cases in this stage:
 *     \$: escapes the dollar sign.
 *     \`: escapes the backtick.
 *     \<newline>: escapes a newline character.
 */
std::string Lexer::consume_double_quote_() {
  std::string text;
  text.reserve(16);

  try {
    while (true) {
      char c = shell_command_stream_.next();
      if (c == '\"') {
        if (shell_command_stream_.peek() == '"') {
          shell_command_stream_.next();
          continue;
        }
        return text;
      }
      if (c == '\\') {
        char pc = shell_command_stream_.peek();
        if (pc == '"' || pc == '\\') {
          text.push_back(shell_command_stream_.next());
          continue;
        }
      }
      text.push_back(c);
    }
  } catch (const std::runtime_error &e) {
    // FIX:Merge two error.
    throw std::runtime_error("[Lexer] Double quote ended unexpectedly\n");
    throw e;
  }
};

void Lexer::consume_normal_() {
  std::string text;
  text.reserve(16);

  auto emplace_word = [&]() {
    if (!text.empty()) {
      parsed_words_.emplace_back(Word{WordType::Text, text});
      text.clear();
    }
  };
  auto consume_space = [&]() {
    while (shell_command_stream_.peek() == ' ')
      shell_command_stream_.next();
  };

  try {
    while (!shell_command_stream_.eof()) {
      char c = shell_command_stream_.next();
      if (c == ' ') {
        emplace_word();
        consume_space();
      } else if (c == '\'') {
        text = text + consume_single_quote_();
      } else if (c == '"') {
        text = text + consume_double_quote_();
      } else {
        text.push_back(c);
      }
    }
    emplace_word();
  } catch (const std::runtime_error &e) {
    throw e;
  }
};

} // namespace parser
} // namespace shell
