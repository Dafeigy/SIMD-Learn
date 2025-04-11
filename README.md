# Accelerate LS Estimation in OAI/openair1 

This branch shows how a specific `for` iter-calculation is speedup. By using SIMD instructions with `_mm256_mullo_epi32` & `_mm256_add_epi32` & `_mm256_sub_epi32`, it approximately accelerate the speed about 5.14x. 

# The fuck it helps?

Simply, it might be able to speed up the calculation of LS estimation located in `openair1/PHY/NR_ESTIMATION/nr_ul_channel_estimation.c`. See the long and sandwitch for iteration struction of LS_estimation calculation, theory it can help configurate about 10ms periodically SRS transmission if you use 13900K. Note that this acceleration is only test in my laptop(12700HK+WSL2@ubuntu20.04) so be careful when merge in your repo.

