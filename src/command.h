#include <algorithm>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>
namespace shell {

class Command;

namespace parser {
enum class WordType { Text, Stdin, Stdout };

struct Word {
  WordType type;
  std::optional<std::string> text;
};

class Lexer {
private:
  std::vector<Word> parsed_;
  std::string_view command_;
  std::string_view::const_iterator current_consume_it_;

  void consume_single_quote_();
  void consume_double_quote_();
  void consume_normal_();
  void consume_all_();

public:
  Lexer() = delete;
  Lexer(std::string_view command); // We will do cosume_ here.

  const std::vector<Word> &words() const;
};
std::vector<Word> lex(std::string_view command);
Command parse(const std::vector<Word> &words);
} // namespace parser
} // namespace shell
