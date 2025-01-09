#include <stdio.h>
#include <stdlib.h>
#include <immintrin.h> // AVX header
#include <time.h>
#include <assert.h>

#define N 1024 // Matrix size

// Native matrix multiplication
void matrix_multiply_native(const float* A, const float* B, float* C, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            float sum = 0.0f;
            for (int k = 0; k < n; ++k) {
                sum += A[i * n + k] * B[k * n + j];
            }
            C[i * n + j] = sum;
        }
    }
}

// AVX matrix multiplication
void matrix_multiply_avx(const float* A, const float* B, float* C, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            __m256 sum = _mm256_setzero_ps(); // Initialize sum to zero

            // Process 8 elements at a time
            int k = 0;
            for (; k <= n - 8; k += 8) {
                __m256 a = _mm256_loadu_ps(&A[i * n + k]); // Load 8 floats from A
                __m256 b = _mm256_loadu_ps(&B[k * n + j]); // Load 8 floats from B (column j)
                sum = _mm256_add_ps(sum, _mm256_mul_ps(a, b)); // Multiply and accumulate
            }

            // Horizontal sum of the 8 elements in sum
            float* sum_array = (float*)&sum;
            float total = 0.0f;
            for (int l = 0; l < 8; ++l) {
                total += sum_array[l];
            }

            // Process remaining elements (if any)
            for (; k < n; ++k) {
                total += A[i * n + k] * B[k * n + j];
            }

            C[i * n + j] = total;
        }
    }
}
