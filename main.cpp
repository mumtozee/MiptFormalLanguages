#include <iostream>
#include <optional>
#include <stack>
#include <string>

#include "Language.h"

enum class RegItem { SYMBOL, PLUS, DOT, KLEENE, ONE, GARBAGE };

RegItem GetTokenType(char c) {
  switch (c) {
    case 'a':  // fall-through
    case 'b':
    case 'c':
      return RegItem::SYMBOL;
    case '+':
      return RegItem::PLUS;
    case '.':
      return RegItem::DOT;
    case '*':
      return RegItem::KLEENE;
    case '1':
      return RegItem::ONE;
    default:
      return RegItem::GARBAGE;
  }
}

template <typename T>
bool VerifyStackSize(const std::stack<T>& stack, RegItem item) {
  switch (item) {
    case RegItem::PLUS:  // fall-through
    case RegItem::DOT:
      return stack.size() >= 2;
    case RegItem::KLEENE:
      return !stack.empty();
    default:
      return true;
  }
}

std::optional<Language> ParseRegexp(const std::string& regexp,
                                    const std::string& word) {
  std::stack<Language> reg_stack;
  Language operand, lhs, rhs;
  for (char c : regexp) {
    auto reg_type = GetTokenType(c);
    switch (reg_type) {
      case RegItem::ONE:  // fall-through
      case RegItem::SYMBOL:
        reg_stack.emplace(c, word);
        break;
      case RegItem::PLUS:
        if (!VerifyStackSize(reg_stack, reg_type)) {
          return {};
        }
        rhs = reg_stack.top();
        reg_stack.pop();
        lhs = reg_stack.top();
        reg_stack.pop();
        reg_stack.push(lhs + rhs);
        break;
      case RegItem::DOT:
        if (!VerifyStackSize(reg_stack, reg_type)) {
          return {};
        }
        rhs = reg_stack.top();
        reg_stack.pop();
        lhs = reg_stack.top();
        reg_stack.pop();
        reg_stack.push(lhs * rhs);
        break;
      case RegItem::KLEENE:
        if (!VerifyStackSize(reg_stack, reg_type)) {
          return {};
        }
        operand = reg_stack.top();
        reg_stack.pop();
        reg_stack.push(operand.Kleene());
        break;
      default:
        break;
    }
  }
  return reg_stack.top();
}

std::optional<size_t> CountMaxSubWordLen(const std::string& regexp,
                                         const std::string& word) {
  auto lang = ParseRegexp(regexp, word);
  if (!lang) {
    return {};
  }
  size_t ans = 0;
  for (size_t i = 0; i < lang->has_prefix_.Size(); ++i) {
    for (size_t j = i; j < lang->has_prefix_.Size(); ++j) {
      if (lang->has_substr_(i, j)) {
        if (j - i > ans) {
          ans = j - i;
        }
      }
    }
  }
  return ans;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  const size_t kInitialCapacity = 500'000;
  std::string regexp;
  regexp.reserve(kInitialCapacity);
  std::string input_word;
  input_word.reserve(kInitialCapacity);
  std::cin >> regexp >> input_word;
  auto result = CountMaxSubWordLen(regexp, input_word);
  if (result) {
    std::cout << *result << std::endl;
  } else {
    std::cout << "INF\n";
  }
  return 0;
}
