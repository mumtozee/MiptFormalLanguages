#ifndef MATRIXBOOL_H
#define MATRIXBOOL_H

#include <queue>
#include <vector>

class MatrixBool {
 public:
  MatrixBool();
  explicit MatrixBool(size_t size, bool value = false);
  MatrixBool(const MatrixBool& other);
  MatrixBool(MatrixBool&& other) noexcept;
  MatrixBool& operator=(const MatrixBool& other);
  MatrixBool& operator=(MatrixBool&& other) noexcept;
  ~MatrixBool();

  std::vector<bool>::const_reference operator()(size_t i, size_t j) const;
  std::vector<bool>::reference operator()(size_t i, size_t j);

  MatrixBool& operator+=(const MatrixBool& other);
  MatrixBool& operator*=(const MatrixBool& other);

  size_t Size() const;

 private:
  std::vector<bool> buffer_;
  size_t size_;
};

MatrixBool operator+(MatrixBool lhs, const MatrixBool& rhs);
MatrixBool operator*(MatrixBool lhs, const MatrixBool& rhs);

void BFS(MatrixBool& graph, size_t root);
void TransitiveClosure(MatrixBool& graph);
void PrintMatrix(const MatrixBool& mtx);

#endif  // MATRIXBOOL_H