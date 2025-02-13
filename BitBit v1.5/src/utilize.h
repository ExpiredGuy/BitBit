#pragma once

#if defined(__linux__)
#include <sys/mman.h>
#endif
#include <cstdlib>

namespace Util {

  inline void* allocAlign(size_t size) {
    constexpr size_t align = 
#if defined(__linux__)
      2 * 1024 * 1024;
#else
      4096;
#endif
    size = (size + align - 1) & ~(align - 1);
    void* result = _mm_malloc(size, align);
#if defined(__linux__)
    madvise(result, size, MADV_HUGEPAGE);
#endif
    return result;
  }

  inline void freeAlign(void* ptr) { _mm_free(ptr); }

}
