# pragma once

# ifdef CRP_USE_SIMD
    # ifdef __x86_64__
        # include <x86intrin.h>
    # endif
# endif