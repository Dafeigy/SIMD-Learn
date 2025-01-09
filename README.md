# Accelerate Matrix Multiplication in Python

This repo contains my learning note about SIMD. I use them in WSL so it should be work as well in Linux. It contains three parts:

## How to use SIMD in AVX-supported Machine

First of all, you need to check if your CPU supports AVX:

```bash
lscpu | grep avx
```
Some information about `AVX` should be shown:
```bash
Flags:
fpu vme ... avx f16c ...
```
If nothing comes up, you can close and say byebye(well actually you can use something like simde, but I don't take a deeper look into it). :(
## How to compile executable & libraray
I write a `compile` script that to compile:
```bash
sudo ./compile
Usage: ./compile [OPTION]
Options:
  --so       Compile ./src/matrix_multiply.c to a shared library (.so)
  --test     Compile ./src/main.c using CMake to an executable
  --all      Compile both the executable and the shared library
  --help     Show this help message
``` 
Compiled files will be store in `./build` folder.
If you don't know where to start, just run 

```bash
sudo ./compile --all
```

## How to use them in Python
Before use them in Python, you need to go through [this part](#how-to-compile-executable--libraray) and make a `.so` library.


Go and check `test.py`. It compares 4 different ways to calculate the matrix multiplication: using Numpy, sing AVX2, Using Native C and using native Python.

The results:

```bash
[512 Dimension] Result: Numpy cost time 0.008693218231201172 seconds
[512 Dimension] Result: C Native cost time 0.06783199310302734 seconds
[512 Dimension] Result: C AVX256 cost time 0.01363515853881836 seconds
[512 Dimension] Result: Native Python cost time 26.925257921218872 seconds
```