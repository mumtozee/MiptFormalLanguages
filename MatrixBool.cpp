#include "MatrixBool.h"

#include <iostream>

MatrixBool::MatrixBool() : MatrixBool(0) {}
MatrixBool::MatrixBool(size_t size, bool value)
    : buffer_(size * size, value), size_(size) {}
MatrixBool::MatrixBool(const MatrixBool& other) = default;
MatrixBool::MatrixBool(MatrixBool&& other) noexcept = default;
MatrixBool& MatrixBool::operator=(const MatrixBool& other) = default;
MatrixBool& MatrixBool::operator=(MatrixBool&& other) noexcept = default;
MatrixBool::~MatrixBool() = default;

std::vector<bool>::const_reference MatrixBool::operator()(size_t i,
                                                          size_t j) const {
  return buffer_[i * size_ + j];
}
std::vector<bool>::reference MatrixBool::operator()(size_t i, size_t j) {
  return buffer_[i * size_ + j];
}

std::vector<bool>::const_reference MatrixBool::Get(size_t i, size_t j) const {
  return buffer_[i * size_ + j];
}
std::vector<bool>::reference MatrixBool::Get(size_t i, size_t j) {
  return buffer_[i * size_ + j];
}

MatrixBool& MatrixBool::operator|=(const MatrixBool& other) {
  if (&other == this) {
    return *this;
  }
  MatrixBool result{size_};
  for (size_t i = 0; i < size_; ++i) {
    for (size_t j = 0; j < size_; ++j) {
      result(i, j) = Get(i, j) || other.Get(i, j);
    }
  }
  return *this = result;
}

MatrixBool& MatrixBool::operator*=(const MatrixBool& other) {
  if (&other == this) {
    return *this = *this * other;
  }
  MatrixBool result{size_};
  for (size_t i = 0; i < size_; ++i) {
    for (size_t j = 0; j < size_; ++j) {
      for (size_t k = i; k <= j; ++k) {
        if (Get(i, k) && other.Get(k, j)) {
          result(i, j) = true;
        }
      }
    }
  }
  return *this = result;
}

size_t MatrixBool::Size() const { return size_; }

MatrixBool operator|(MatrixBool lhs, const MatrixBool& rhs) {
  return lhs |= rhs;
}

MatrixBool operator*(MatrixBool lhs, const MatrixBool& rhs) {
  return lhs *= rhs;
}

void MatrixBool::BFS(size_t root) {
  std::vector<bool> visited(Size(), false);
  std::queue<size_t> vq;
  vq.push(root);
  visited[root] = true;
  while (!vq.empty()) {
    auto curr = vq.front();
    vq.pop();
    Get(root, curr) = true;
    for (size_t i = 0; i < Size(); ++i) {
      if (Get(curr, i) && !visited[i]) {
        visited[i] = true;
        vq.push(i);
      }
    }
  }
}

void MatrixBool::PrintMatrix() const {
  for (size_t i = 0; i < Size(); ++i) {
    for (size_t j = 0; j < Size(); ++j) {
      std::cout << Get(i, j) << ' ';
    }
    std::cout << '\n';
  }
  std::cout << '\n';
}

MatrixBool MatrixBool::TransitiveClosure() const {
  MatrixBool graph{*this};
  for (size_t i = 0; i < graph.Size(); ++i) {
    graph.BFS(i);
  }
  return graph;
}
