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

MatrixBool& MatrixBool::operator+=(const MatrixBool& other) {
  if (&other == this) {
    return *this;
  }
  MatrixBool result{size_};
  for (size_t i = 0; i < size_; ++i) {
    for (size_t j = 0; j < size_; ++j) {
      result(i, j) =
          buffer_[i * size_ + j] || other.buffer_[i * size_ + j];
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
        if (buffer_[i * size_ + k] && other.buffer_[k * size_ + j]) {
          result(i, j) = true;
        }
      }
    }
  }
  return *this = result;
}

size_t MatrixBool::Size() const { return size_; }

MatrixBool operator+(MatrixBool lhs, const MatrixBool& rhs) {
  return lhs += rhs;
}

MatrixBool operator*(MatrixBool lhs, const MatrixBool& rhs) {
  return lhs *= rhs;
}

void BFS(MatrixBool& graph, size_t root) {
  std::vector<bool> visited(graph.Size(), false);
  std::queue<size_t> vq;
  vq.push(root);
  visited[root] = true;
  while (!vq.empty()) {
    auto curr = vq.front();
    vq.pop();
    graph(root, curr) = true;
    for (size_t i = 0; i < graph.Size(); ++i) {
      if (graph(curr, i) && !visited[i]) {
        visited[i] = true;
        vq.push(i);
      }
    }
  }
}

void PrintMatrix(const MatrixBool& mtx) {
  for (size_t i = 0; i < mtx.Size(); ++i) {
    for (size_t j = 0; j < mtx.Size(); ++j) {
      std::cout << mtx(i, j) << ' ';
    }
    std::cout << '\n';
  }
  std::cout << '\n';
}

void TransitiveClosure(MatrixBool& graph) {
  for (size_t i = 0; i < graph.Size(); ++i) {
    BFS(graph, i);
  }
}