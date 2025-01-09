#include <stdio.h>
#include <stdlib.h>
#include "matrix_multiply.c"

int main() {
    int n = N; // Matrix size

    // Allocate aligned memory for matrices
    float* A = (float*)_mm_malloc(n * n * sizeof(float), 32);
    float* B = (float*)_mm_malloc(n * n * sizeof(float), 32);
    float* C_native = (float*)_mm_malloc(n * n * sizeof(float), 32);
    float* C_avx = (float*)_mm_malloc(n * n * sizeof(float), 32);

    assert(A != NULL && B != NULL && C_native != NULL && C_avx != NULL);

    // Initialize matrices A and B
    for (int i = 0; i < n * n; ++i) {
        A[i] = 1.0f; // Example: All elements are 1
        B[i] = 1.0f; // Example: All elements are 1
    }

    // Native matrix multiplication
    clock_t start_native = clock();
    matrix_multiply_native(A, B, C_native, n);
    clock_t end_native = clock();
    double duration_native = (double)(end_native - start_native) / CLOCKS_PER_SEC;
    printf("Native matrix multiplication time: %f seconds\n", duration_native);

    // AVX matrix multiplication
    clock_t start_avx = clock();
    matrix_multiply_avx(A, B, C_avx, n);
    clock_t end_avx = clock();
    double duration_avx = (double)(end_avx - start_avx) / CLOCKS_PER_SEC;
    printf("AVX matrix multiplication time: %f seconds\n", duration_avx);

    // Verify results
    for (int i = 0; i < n * n; ++i) {
        if (C_native[i] != C_avx[i]) {
            fprintf(stderr, "Error: Results do not match at index %d\n", i);
            break;
        }
    }

    // Free memory
    _mm_free(A);
    _mm_free(B);
    _mm_free(C_native);
    _mm_free(C_avx);

    return 0;
}