import ctypes
import time
import numpy as np
# 加载动态库
lib = ctypes.CDLL('./build/libmatrix.so')

# 配置 argtypes
matrix_multiply_native = lib.matrix_multiply_native
lib.matrix_multiply_native.argtypes = [
    ctypes.POINTER(ctypes.c_float), 
    ctypes.POINTER(ctypes.c_float), 
    ctypes.POINTER(ctypes.c_float), 
    ctypes.c_int]
lib.matrix_multiply_native.restype = None
# 定义数组大小
n = 512

# 创建输入数组 A 和 B
A = (ctypes.c_float * (n*n))(*[1.0 for _ in range(n*n)])
B = (ctypes.c_float * (n*n))(*[1.0 for _ in range(n*n)])
C = (ctypes.c_float * (n*n))()

# 将 CTypes 数组转换为 NumPy 数组
A_np = np.ctypeslib.as_array(A).reshape(n, n)
B_np = np.ctypeslib.as_array(B).reshape(n, n)

# 使用 NumPy 计算矩阵乘法
st0 = time.time()
C_np = np.dot(A_np, B_np)
# 将结果转换回 CTypes 数组
print(f"[{n} Dimension] Result: Numpy cost time {time.time() - st0} seconds")
C = (ctypes.c_float * (n*n))(*C_np.flatten().tolist())
##################

# 调用C函数
st1 = time.time()
lib.matrix_multiply_native(A, B, C, n)

# 打印结果
print(f"[{n} Dimension] Result: C Native cost time {time.time() - st1} seconds")
# for i in range(n):
#     for j in range(n):
#         print(C[i * n + j], end=' ')
#     print()

A = (ctypes.c_float * (n*n))(*[1.0 for _ in range(n*n)])
B = (ctypes.c_float * (n*n))(*[1.0 for _ in range(n*n)])
C = (ctypes.c_float * (n*n))()
total = n*n*n
matrix_multiply_avx = lib.matrix_multiply_avx
lib.matrix_multiply_avx.argtypes = [
    ctypes.POINTER(ctypes.c_float), 
    ctypes.POINTER(ctypes.c_float), 
    ctypes.POINTER(ctypes.c_float), 
    ctypes.c_int]
lib.matrix_multiply_avx.restype = None
st2 = time.time()
lib.matrix_multiply_avx(A, B, C, n)
# 打印结果
print(f"[{n} Dimension] Result: C AVX256 cost time {time.time() - st2} seconds")

A = [[1.0 for _ in range(n)] for j in range(n)]
B = [[1.0 for _ in range(n)] for j in range(n)]
C = [[0.0 for _ in range(n)] for j in range(n)]
st3 = time.time()
for i in range(n):
    for j in range(n):
        for k in range(n):
            # print(f"{n*n*i + n*j + k}/{total}")
            C[i][j] += A[i][k] * B[j][k]
# 打印结果
print(f"[{n} Dimension] Result: Native Python cost time {time.time() - st3} seconds")
# for i in range(n):
#     for j in range(n):
#         print(C[i * n + j], end=' ')
#     print()
