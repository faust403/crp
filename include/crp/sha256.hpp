# pragma once

# include <array>
# include <string>
# include <cstdint>
# include <cstring>
# include <crp/utils.hpp>

# ifdef CRP_USE_SIMD
    # include <crp/simd.hpp>
# endif

# include <iostream>

# define CRP_SHA256_LENGTH_BITS 2'048
# define CRP_SHA256_LENGTH_BYTES 256
# define CRP_SHA256_LENGTH_PART 32
# define CRP_SHA256_32BYTES_BLOCKS_COUNT 8

enum class CRP_FORMAT : std::uint8_t {
    upper,
    lower
};

std::uint32_t* raw_sha256(const void*, const std::uint64_t) noexcept;
std::string sha256(const void*, const std::uint64_t, const std::string = "", const CRP_FORMAT = CRP_FORMAT::lower) noexcept;