// matrix_multiply.h
#ifndef MATRIX_MULTIPLY_H
#define MATRIX_MULTIPLY_H

void matrix_multiply_native(const float* A, const float* B, float* C, int n);
void matrix_multiply_avx(const float* A, const float* B, float* C, int n);

#endif // MATRIX_MULTIPLY_H