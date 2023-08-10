# pragma once

# ifdef CRP_USE_MIMALLOC
    # include <mimalloc.h>
    # define crp_align_allocate(SIZE, ALIGN) mi_malloc_aligned(SIZE, ALIGN)

    # define crp_allocate(BYTES) mi_malloc(BYTES)
    # define crp_free(POINTER) mi_free(POINTER)
# else
    # include <malloc.h>
    # define crp_align_allocate(SIZE, ALIGN) memalign(ALIGN, SIZE)
    
    # define crp_allocate(BYTES) malloc(BYTES)
    # define crp_free(POINTER) free(POINTER)
# endif
