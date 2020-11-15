#ifndef LANGUAGE_H
#define LANGUAGE_H

#include <string>

#include "MatrixBool.h"

struct Language {
  MatrixBool has_whole_;
  MatrixBool has_prefix_;
  MatrixBool has_suffix_;
  MatrixBool has_substr_;

  Language();
  Language(char c, const std::string& word);
  Language(const Language& other);
  Language(Language&& other) noexcept;
  Language& operator=(const Language& other);
  Language& operator=(Language&& other) noexcept;
  ~Language();

  Language& operator+=(const Language& other);

  Language& operator*=(const Language& other);

  Language Kleene() const;
};

Language operator+(Language lhs, const Language& rhs);
Language operator*(Language lhs, const Language& rhs);

#endif  // LANGUAGE_H
