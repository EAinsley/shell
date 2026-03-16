#include "command.h"
#include <optional>
#include <stdexcept>
#include <vector>

namespace shell {
namespace parser {

/** ShellCommandStream **/
char ShellCommandStream::next() {
  if (eof()) {
    throw std::runtime_error("[SelleCommandStream] consume eof.\n");
  }
  return command_[++current_pos_];
}

char ShellCommandStream::peek() const {
  if (current_pos_ + 1 < command_.size()) {
    return command_[current_pos_ + 1];
  }
  return '\0';
}

char ShellCommandStream::now() const {
  if (!eof() && current_pos_ != -1) {
    return command_[current_pos_];
  }
  return '\0';
}
bool ShellCommandStream::eof() const {
  return current_pos_ + 1 >= command_.size();
}

/** Lexer **/
Lexer::Lexer(std::string_view command) : shell_command_stream_(command) {
  consume_normal_();
}

const WordList Lexer::words_str() const {
  WordList res;
  for (const auto &word : this->parsed_words_) {
    if (word.text.has_value()) {
      res.emplace_back(word.text.value());
    }
  }
  return res;
}

const std::vector<Word> &Lexer::words() const { return parsed_words_; }
const WordType Lexer::next() {
  if (eof()) {
    throw std::runtime_error("[Lexer] consum eof.\n");
  }
  return parsed_words_[++pos_].type;
}
const WordType Lexer::peek() const {
  if (pos_ + 1 < parsed_words_.size()) {
    return parsed_words_[pos_ + 1].type;
  }
  return WordType::Eof;
}
const WordType Lexer::now() const {
  if (!eof() && pos_ != -1) {
    return parsed_words_[pos_].type;
  }
  return WordType::Eof;
}

const std::optional<std::string> Lexer::word() const {
  if (!eof() && pos_ != -1) {
    return parsed_words_[pos_].text;
  }
  return std::nullopt;
}

bool Lexer::eof() const { return pos_ + 1 >= parsed_words_.size(); }

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
  auto emplace_type = [&](WordType type) {
    parsed_words_.emplace_back(Word{type, std::nullopt});
    text.clear();
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
      } else if (c == '>') {
        // TODO: Do we need to put this int to functions?
        if (text.length() == 1 && text[0] == '2') {
          emplace_type(WordType::Stderr);
        } else {
          emplace_word();
          emplace_type(WordType::Stdin);
        }
      }

      else {
        text.push_back(c);
      }
    }
    emplace_word();
    emplace_type(WordType::Eof);
  } catch (const std::runtime_error &e) {
    throw e;
  }
};

AbstractCommand parse(std::string_view command) {
  AbstractCommand res;
  Lexer lexer(command);
  if (lexer.next() != WordType::Text) {
    throw std::runtime_error("[Parser] Expect string.");
  }
  res.command = lexer.word().value();
  while (!lexer.eof()) {
    WordType wt = lexer.next();
    if (wt == WordType::Text) {
      res.arguments.emplace_back(lexer.word().value());
    } else if (wt == WordType::Stdin || wt == WordType::Stderr) {
      if (lexer.next() != WordType::Text) {
        throw std::runtime_error("[Parser] Redirector expect filename");
      }
      Redirector rd;
      rd.type = RedirectorType::Write;
      if (wt == WordType::Stdin) {
        rd.target_fd = 1;
      } else {
        rd.target_fd = 2;
      }
      rd.filename = lexer.word().value();
      res.redirectors.emplace_back(std::move(rd));
    }
  }
  return res;
}

} // namespace parser
} // namespace shell
