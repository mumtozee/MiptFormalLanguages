#include <iostream>
#include <stack>
#include <string>

#include "Language.h"

bool CheckRegexpValidity(const std::string& regexp) {
  size_t stack_size = 0;
  for (char symbol : regexp) {
    if (symbol == 'a' || symbol == 'b' || symbol == 'c' || symbol == '1') {
      ++stack_size;
    } else if (symbol == '+' || symbol == '.') {
      if (stack_size < 2) {
        return false;
      }
      --stack_size;
    } else if (symbol == '*') {
      if (stack_size < 1) {
        return false;
      }
    } else {
      return false;
    }
  }
  return stack_size == 1;
}

Language ParseRegexp(const std::string& regexp, const std::string& word) {
  /* check regexp validity before using this function */
  std::stack<Language> reg_stack;
  Language operand, lhs, rhs;
  for (char c : regexp) {
    switch (c) {
      case 'a':
        reg_stack.emplace(c, word);
        break;
      case 'b':
        reg_stack.emplace(c, word);
        break;
      case 'c':
        reg_stack.emplace(c, word);
        break;
      case '1':
        reg_stack.emplace(c, word);
        break;
      case '+':
        rhs = reg_stack.top();
        reg_stack.pop();
        lhs = reg_stack.top();
        reg_stack.pop();
        reg_stack.push(lhs + rhs);
        break;
      case '.':
        rhs = reg_stack.top();
        reg_stack.pop();
        lhs = reg_stack.top();
        reg_stack.pop();
        reg_stack.push(lhs * rhs);
        break;
      case '*':
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

size_t CountMaxSubWordLen(const std::string& regexp, const std::string& word) {
  Language lang = ParseRegexp(regexp, word);
  size_t ans = 0;
  for (size_t i = 0; i < lang.has_prefix_.Size(); ++i) {
    for (size_t j = i; j < lang.has_prefix_.Size(); ++j) {
      if (lang.has_substr_(i, j)) {
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
  if (CheckRegexpValidity(regexp)) {
    std::cout << CountMaxSubWordLen(regexp, input_word) << std::endl;
  } else {
    std::cout << "INF\n";
  }
  return 0;
}