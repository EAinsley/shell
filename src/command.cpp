#include "command.h"
#include <stdexcept>
#include <vector>

namespace shell {
namespace parser {

Lexer::Lexer(std::string_view command)
    : command_(command), current_consume_it_(command.cbegin()) {
  consume_all_();
}

const std::vector<Word> &Lexer::words() const { return parsed_; }
void Lexer::consume_single_quote_() {
  bool is_escaping = false;
  std::string text;
  text.reserve(16);

  for (; current_consume_it_ != command_.end(); ++current_consume_it_) {
    if (is_escaping) {
      is_escaping = false;
      text.push_back(*current_consume_it_);
      continue;
    }

    if (*current_consume_it_ == '\'' && !text.empty()) {
      parsed_.emplace_back(Word{WordType::Text, text});
      return;
    }
    if (*current_consume_it_ == '\\') {
      is_escaping = true;
      continue;
    }
    text.push_back(*current_consume_it_);
  }
  throw std::runtime_error("Single quote ended unexpectedly");
}

void Lexer::consume_double_quote_() {
  bool is_escaping = false;
  std::string text;
  text.reserve(16);
  for (; current_consume_it_ != command_.end(); ++current_consume_it_) {
    if (is_escaping) {
      is_escaping = false;
      if (*current_consume_it_ != '"' && *current_consume_it_ != '\\' &&
          *current_consume_it_ != '$' && *current_consume_it_ != '`') {
        text.push_back('\\');
      }
      text.push_back(*current_consume_it_);
      continue;
    }

    if (*current_consume_it_ == '"' && !text.empty()) {
      parsed_.emplace_back(Word{WordType::Text, text});
      return;
    }
    if (*current_consume_it_ == '\\') {
      is_escaping = true;
      continue;
    }
    text.push_back(*current_consume_it_);
  }
  throw std::runtime_error("Double quote ended unexpectedly");
};

void Lexer::consume_normal_() {
  bool is_escaping = false;
  std::string text;
  text.reserve(16);
  for (; current_consume_it_ != command_.end(); ++current_consume_it_) {
    if (is_escaping) {
      is_escaping = false;
      text.push_back(*current_consume_it_);
      continue;
    }

    if (*current_consume_it_ == ' ') {
      if (!text.empty()) {
        parsed_.emplace_back(Word{WordType::Text, text});
        text.clear();
      }
      continue;
    }

    if (*current_consume_it_ == '\'' || *current_consume_it_ == '"') {
      return;
    }

    if (*current_consume_it_ == '\\') {
      is_escaping = true;
      continue;
    }
    text.push_back(*current_consume_it_);
  }
  throw std::runtime_error("Double quote ended unexpectedly");
};
void Lexer::consume_all_() {};

std::vector<Word> lex(std::string_view command) {

  std::vector<Word> wordlist;

  auto consume_single_quote = [&command,
                               &wordlist](std::string_view::iterator &it) {
    bool is_escaping = false;
    std::string text;
    text.reserve(16);

    for (; it != command.end(); ++it) {
      if (is_escaping) {
        is_escaping = false;
        text.push_back(*it);
        continue;
      }

      if (*it == '\'' && !text.empty()) {
        wordlist.emplace_back(Word{WordType::Text, text});
        return;
      }
      if (*it == '\\') {
        is_escaping = true;
        continue;
      }
      text.push_back(*it);
    }
    throw std::runtime_error("Single quote ended unexpectedly");
  };

  auto consume_double_qoute = [&command,
                               &wordlist](std::string_view::iterator &it) {
    bool is_escaping = false;
    std::string text;
    text.reserve(16);

    for (; it != command.end(); ++it) {
      if (is_escaping) {
        is_escaping = false;
        if (*it != '"' && *it != '\\' && *it != '$' && *it != '`') {
          text.push_back('\\');
        }
        text.push_back(*it);
        continue;
      }

      if (*it == '"' && !text.empty()) {
        wordlist.emplace_back(Word{WordType::Text, text});
        return;
      }
      if (*it == '\\') {
        is_escaping = true;
        continue;
      }
      text.push_back(*it);
    }
    throw std::runtime_error("Double quote ended unexpectedly");
  };
}
}

} // namespace parser
} // namespace shell
