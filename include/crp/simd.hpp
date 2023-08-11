# pragma once

# ifdef CRP_USE_SIMD
    # ifdef __x86_64__
        # include <x86intrin.h>
        # include <crp/align.hpp>

        # define crp_64 __m64
        # define crp_128 __m128
        # define crp_128i __m128i
        # define crp_set_16_8(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p) _mm_set_epi8(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p)
        # define crp_set_8_16(a, b, c, d, e, f, g, h) _mm_set_epi16(a, b, c, d, e, f, g, h)
        # define crp_set_4_32(a, b, c, d) _mm_set_epi32(a, b, c, d)
        # define crp_set1_4_32(a, b, c, d) _mm_set1_epi32(a, b, c, d)
        # define crp_set_2_64(a, b) _mm_set_epi_64(a, b)

        # define crp_zero_128() _mm_setzero_si128()
        # define crp_store_aligned_128(Address, Vector) _mm_store_si128(Address, Vector)
        # define crp_store_unaligned_128(Address, Vector) _mm_storeu_si128(Address, Vector)

        # define crp_add_4_32(a, b) _mm_add_epi32(a, b)
    # endif
# endif