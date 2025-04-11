#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <immintrin.h>

// 优化前的函数
void original_complex_multiply(const int* generated_real, const int* received_real,
                               const int* generated_imag, const int* received_imag,
                               int* ls_estimated_r, int* ls_estimated_i, int length) {
    for (int i = 0; i < length; i++) {
        ls_estimated_r[i] = generated_real[i] * received_real[i] + generated_imag[i] * received_imag[i];
        ls_estimated_i[i] = generated_real[i] * received_imag[i] - generated_imag[i] * received_real[i];
    }
}

// 优化后的函数
void optimized_complex_multiply(const int* generated_real, const int* received_real,
                                const int* generated_imag, const int* received_imag,
                                int* ls_estimated_r, int* ls_estimated_i, int length) {
    int i;
    for (i = 0; i + 7 < length; i += 8) {
        // 加载数据到AVX寄存器
        __m256i gen_real = _mm256_loadu_si256((__m256i*)&generated_real[i]);
        __m256i rec_real = _mm256_loadu_si256((__m256i*)&received_real[i]);
        __m256i gen_imag = _mm256_loadu_si256((__m256i*)&generated_imag[i]);
        __m256i rec_imag = _mm256_loadu_si256((__m256i*)&received_imag[i]);

        // 计算实部：generated_real * received_real + generated_imag * received_imag
        __m256i real_part1 = _mm256_mullo_epi32(gen_real, rec_real);
        __m256i real_part2 = _mm256_mullo_epi32(gen_imag, rec_imag);
        __m256i result_r = _mm256_add_epi32(real_part1, real_part2);

        // 计算虚部：generated_real * received_imag - generated_imag * received_real
        __m256i imag_part1 = _mm256_mullo_epi32(gen_real, rec_imag);
        __m256i imag_part2 = _mm256_mullo_epi32(gen_imag, rec_real);
        __m256i result_i = _mm256_sub_epi32(imag_part1, imag_part2);

        // 存储结果
        _mm256_storeu_si256((__m256i*)&ls_estimated_r[i], result_r);
        _mm256_storeu_si256((__m256i*)&ls_estimated_i[i], result_i);
    }

    // 处理剩余的元素
    for (; i < length; i++) {
        ls_estimated_r[i] = generated_real[i] * received_real[i] + generated_imag[i] * received_imag[i];
        ls_estimated_i[i] = generated_real[i] * received_imag[i] - generated_imag[i] * received_real[i];
    }
}

// 测试函数
void run_tests(int length, int iterations) {
    int *generated_real = (int *)malloc(length * sizeof(int));
    int *received_real = (int *)malloc(length * sizeof(int));
    int *generated_imag = (int *)malloc(length * sizeof(int));
    int *received_imag = (int *)malloc(length * sizeof(int));
    int *ls_estimated_r = (int *)malloc(length * sizeof(int));
    int *ls_estimated_i = (int *)malloc(length * sizeof(int));

    // 随机生成数据
    for (int i = 0; i < length; i++) {
        generated_real[i] = rand() % 100; // 生成0到99之间的随机整数
        received_real[i] = rand() % 100;
        generated_imag[i] = rand() % 100;
        received_imag[i] = rand() % 100;
    }

    clock_t start, end;
    double original_time = 0.0, optimized_time = 0.0;

    // 测试优化前的函数
    for (int i = 0; i < iterations; i++) {
        start = clock();
        original_complex_multiply(generated_real, received_real, generated_imag, received_imag, ls_estimated_r, ls_estimated_i, length);
        end = clock();
        original_time += (double)(end - start) / CLOCKS_PER_SEC;
    }

    // 测试优化后的函数
    for (int i = 0; i < iterations; i++) {
        start = clock();
        optimized_complex_multiply(generated_real, received_real, generated_imag, received_imag, ls_estimated_r, ls_estimated_i, length);
        end = clock();
        optimized_time += (double)(end - start) / CLOCKS_PER_SEC;
    }

    // 输出结果
    printf("Original function average time: %.6f seconds\n", original_time / iterations);
    printf("Optimized function average time: %.6f seconds\n", optimized_time / iterations);
    printf("Speedup: %.2fx\n", original_time / optimized_time);

    // 释放内存
    free(generated_real);
    free(received_real);
    free(generated_imag);
    free(received_imag);
    free(ls_estimated_r);
    free(ls_estimated_i);
}

int main() {
    srand(time(NULL)); // 初始化随机种子

    int length = 624; // 数组长度
    int iterations = 1000000; // 测试轮数

    printf("Running tests with array length %d and %d iterations...\n", length, iterations);
    run_tests(length, iterations);

    return 0;
}
