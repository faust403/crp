# pragma once

# include <array>
# include <string>
# include <cstdint>
# include <cstring>
# include <crp/utils.hpp>

# include <iostream>

# define CRP_SHA256_LENGTH_BITS 2'048
# define CRP_SHA256_LENGTH_BYTES 256
# define CRP_SHA256_LENGTH_PART 32
# define CRP_SHA256_32BYTES_BLOCKS_COUNT 8

std::uint32_t* raw_sha256(const void*, const std::uint64_t) noexcept;
std::string sha256(const void*, const std::uint64_t) noexcept;