# pragma once

# if defined(__linux__) || defined(__APPLE__)
    # include <bits/wordsize.h>
    # define  crp_wordsize __WORDSIZE
    # define crp_align(a) alignas(a)
# elif defined(_WIN64)
    # define crp_wordsize 64 // todo
    # define crp_align(a) __declspec(align(a))
# endif