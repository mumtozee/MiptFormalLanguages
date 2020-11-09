#include "Language.h"

Language::Language() = default;

Language::Language(char c, const std::string& word)
    : has_whole_(word.size() + 1),
      has_prefix_(word.size() + 1),
      has_suffix_(word.size() + 1),
      has_substr_(word.size() + 1) {
  for (size_t i = 0; i < word.size() + 1; ++i) {
    has_prefix_(i, i) = true;
    has_suffix_(i, i) = true;
    has_substr_(i, i) = true;
    if (c == '1') {
      has_whole_(i, i) = true;
    }
  }

  for (size_t i = 0; i < word.size(); ++i) {
    if (word[i] == c) {
      has_prefix_(i, i + 1) = true;
      has_suffix_(i, i + 1) = true;
      has_substr_(i, i + 1) = true;
      has_whole_(i, i + 1) = true;
    }
  }
}

Language::Language(const Language& other) = default;
Language::Language(Language&& other) noexcept = default;
Language& Language::operator=(const Language& other) = default;
Language& Language::operator=(Language&& other) noexcept = default;
Language::~Language() = default;

Language& Language::operator+=(const Language& other) {
  if (&other == this) {
    return *this;
  }
  has_prefix_ += other.has_prefix_;
  has_suffix_ += other.has_suffix_;
  has_whole_ += other.has_whole_;
  has_substr_ += other.has_substr_;
  return *this;
}

Language& Language::operator*=(const Language& other) {
  if (&other == this) {
    return *this = *this * other;
  }
  has_prefix_ += has_whole_ * other.has_prefix_;
  has_suffix_ = has_suffix_ * other.has_whole_ + other.has_suffix_;
  has_whole_ *= other.has_whole_;
  has_substr_ += has_suffix_ * other.has_prefix_ + other.has_substr_;
  return *this;
}

Language Language::Kleene() const {
  Language result;
  result.has_whole_ = has_whole_;
  TransitiveClosure(result.has_whole_);
  result.has_prefix_ = result.has_whole_ * has_prefix_;
  result.has_suffix_ = has_suffix_ * result.has_whole_;
  result.has_substr_ =
      has_suffix_ * result.has_whole_ * has_prefix_ + has_substr_;
  return result;
}

Language operator+(Language lhs, const Language& rhs) { return lhs += rhs; }
Language operator*(Language lhs, const Language& rhs) { return lhs *= rhs; }