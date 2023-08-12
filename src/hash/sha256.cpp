// https://helix.stormhub.org/papers/SHA-256.pdf

# include <crp/sha256.hpp>

[[ nodiscard ]] static inline std::uint32_t ch(const std::uint32_t& x, const std::uint32_t& y, const std::uint32_t& z) noexcept
{
    return (x & y) ^ (~x & z);
}

[[ nodiscard ]] static inline std::uint32_t maj(const std::uint32_t& x, const std::uint32_t& y, const std::uint32_t& z) noexcept
{
    return (x & y) ^ (x & z) ^ (y & z);
}

[[ nodiscard ]] static inline std::uint32_t ROTR(const std::uint32_t& x, const std::uint8_t& r) noexcept
{
    return 0x0 | (x >> r) | (x << (8 * (sizeof x) - r));
}

[[ nodiscard ]] static inline std::uint32_t S0(const std::uint32_t& x) noexcept
{
    return ROTR(x, 2) ^ ROTR(x, 13) ^ ROTR(x, 22);
}

[[ nodiscard ]] static inline std::uint32_t S1(const std::uint32_t& x) noexcept
{
    return ROTR(x, 6) ^ ROTR(x, 11) ^ ROTR(x, 25);
}

[[ nodiscard ]] static inline std::uint32_t s0(const std::uint32_t& x) noexcept
{
    return ROTR(x, 7) ^ ROTR(x, 18) ^ (x >> 3);
}

[[ nodiscard ]] static inline std::uint32_t s1(const std::uint32_t& x) noexcept
{
    return ROTR(x, 17) ^ ROTR(x, 19) ^ (x >> 10);
}

static void update (
    # ifndef CRP_USE_SIMD
        std::uint32_t* Result,
    # else
        crp_128i* Result,
    # endif
    const std::uint8_t* From
) noexcept
{
    constexpr static std::uint32_t Constants[] = {
        0x428A2F98, 0x71374491, 0xB5C0FBCF, 0xE9B5DBA5, 0x3956C25B, 0x59F111F1, 0x923F82A4, 0xAB1C5ED5,
        0xD807AA98, 0x12835B01, 0x243185BE, 0x550C7DC3, 0x72BE5D74, 0x80DEB1FE, 0x9BDC06A7, 0xC19BF174,
        0xE49B69C1, 0xEFBE4786, 0x0FC19DC6, 0x240CA1CC, 0x2DE92C6F, 0x4A7484AA, 0x5CB0A9DC, 0x76F988DA,
        0x983E5152, 0xA831C66D, 0xB00327C8, 0xBF597FC7, 0xC6E00BF3, 0xD5A79147, 0x06CA6351, 0x14292967,
        0x27B70A85, 0x2E1B2138, 0x4D2C6DFC, 0x53380D13, 0x650A7354, 0x766A0ABB, 0x81C2C92E, 0x92722C85,
        0xA2BFE8A1, 0xA81A664B, 0xC24B8B70, 0xC76C51A3, 0xD192E819, 0xD6990624, 0xF40E3585, 0x106AA070,
        0x19A4C116, 0x1E376C08, 0x2748774C, 0x34B0BCB5, 0x391C0CB3, 0x4ED8AA4A, 0x5B9CCA4F, 0x682E6FF3,
        0x748F82EE, 0x78A5636F, 0x84C87814, 0x8CC70208, 0x90BEFFFA, 0xA4506CEB, 0xBEF9A3F7, 0xC67178F2
    };

    # ifndef CRP_USE_SIMD
        static std::uint32_t Schedule[64];
        std::memset(Schedule, 0x0, 64);   

        std::uint8_t Iter = 0;
        for(std::uint8_t Kter = 0x0; Iter < 64; Iter += 4, Kter += 1)
            Schedule[Kter] = static_cast<std::uint32_t>(From[Iter]) << 24
                        |  static_cast<std::uint32_t>(From[Iter + 1]) << 16
                        |  static_cast<std::uint32_t>(From[Iter + 2]) << 8
                        |  static_cast<std::uint32_t>(From[Iter + 3]);

        for(Iter = 16; Iter < 64; Iter += 1)
            Schedule[Iter] = s1(Schedule[Iter - 2]) + Schedule[Iter - 7] + s0(Schedule[Iter - 15]) + Schedule[Iter - 16];

        std::uint32_t a = Result[0];
        std::uint32_t b = Result[1];
        std::uint32_t c = Result[2];
        std::uint32_t d = Result[3];
        std::uint32_t e = Result[4];
        std::uint32_t f = Result[5];
        std::uint32_t g = Result[6];
        std::uint32_t h = Result[7];

        for(Iter = 0x0; Iter < 64; Iter += 1)
        {
            const std::uint32_t tmp1 = h + S1(e) + ch(e, f, g) + Constants[Iter] + Schedule[Iter];
            const std::uint32_t tmp2 = S0(a) + maj(a, b, c);
            
            h = g;
            g = f;
            f = e;
            e = d + tmp1;
            d = c;
            c = b;
            b = a;
            a = tmp1 + tmp2;
        }

        Result[0] += a;
        Result[1] += b;
        Result[2] += c;
        Result[3] += d;
        Result[4] += e;
        Result[5] += f;
        Result[6] += g;
        Result[7] += h;
    # else
        std::uint64_t Iter = 0x0;
        crp_align(crp_wordsize) static std::uint32_t Schedule[64];
        const static crp_128 ZeroVector = crp_zero_128();
        
        # pragma omp simd safelen(4)
        for(; Iter < 64; Iter += 4)
            crp_store_aligned_128((crp_128i*)&Schedule[Iter], ZeroVector);

        Iter = 0x0;
        # pragma omp simd safelen(4)
        for(std::uint8_t Kter = 0x0; Iter < 64; Iter += 16, Kter += 4)
        {
            const crp_128i SettedLines4 = crp_set_16_8 (
                static_cast<char>(From[Iter + 12]),
                static_cast<char>(From[Iter + 13]),
                static_cast<char>(From[Iter + 14]),
                static_cast<char>(From[Iter + 15]),

                static_cast<char>(From[Iter + 8]),
                static_cast<char>(From[Iter + 9]),
                static_cast<char>(From[Iter + 10]),
                static_cast<char>(From[Iter + 11]),

                static_cast<char>(From[Iter + 4]),
                static_cast<char>(From[Iter + 5]),
                static_cast<char>(From[Iter + 6]),
                static_cast<char>(From[Iter + 7]),

                static_cast<char>(From[Iter]),
                static_cast<char>(From[Iter + 1]),
                static_cast<char>(From[Iter + 2]),
                static_cast<char>(From[Iter + 3])
            );
            crp_store_aligned_128((crp_128i*)&Schedule[Kter], SettedLines4);
        }

        # pragma omp simd
        for(Iter = 16; Iter < 64; Iter += 4)
        {
            Schedule[Iter] = s1(Schedule[Iter - 2]) + Schedule[Iter - 7] + s0(Schedule[Iter - 15]) + Schedule[Iter - 16];
            Schedule[Iter + 1] = s1(Schedule[Iter - 1]) + Schedule[Iter - 6] + s0(Schedule[Iter - 14]) + Schedule[Iter - 15];
            Schedule[Iter + 2] = s1(Schedule[Iter]) + Schedule[Iter - 5] + s0(Schedule[Iter - 13]) + Schedule[Iter - 14];
            Schedule[Iter + 3] = s1(Schedule[Iter + 1]) + Schedule[Iter - 4] + s0(Schedule[Iter - 12]) + Schedule[Iter - 13];
        }

        crp_align(crp_wordsize) std::uint32_t Intermediate[8];
        crp_store_aligned_128 ((crp_128i*)&Intermediate[0], Result[0]);
        crp_store_aligned_128 ((crp_128i*)&Intermediate[4], Result[1]);
        # pragma omp simd
        for(Iter = 0x0; Iter < 64; Iter += 1)
        {
            const std::uint32_t tmp1 = Intermediate[7] + S1(Intermediate[4]) + ch(Intermediate[4], Intermediate[5], Intermediate[6]) + Constants[Iter] + Schedule[Iter];
            const std::uint32_t tmp2 = S0(Intermediate[0]) + maj(Intermediate[0], Intermediate[1], Intermediate[2]);

            Intermediate[7] = Intermediate[6];
            Intermediate[6] = Intermediate[5];
            Intermediate[5] = Intermediate[4];
            Intermediate[4] = Intermediate[3] + tmp1;
            Intermediate[3] = Intermediate[2];
            Intermediate[2] = Intermediate[1];
            Intermediate[1] = Intermediate[0];
            Intermediate[0] = tmp1 + tmp2;
        }

        crp_128i ResultVector0_3 = Result[0];
        crp_128i ResultVector4_7 = Result[1];
        crp_128i NumbersVector0_3 = *(crp_128i*)&Intermediate[0];
        crp_128i NumbersVector4_7 = *(crp_128i*)&Intermediate[4];
        crp_128i Results0_3 = crp_add_4_32(ResultVector0_3, NumbersVector0_3);
        crp_128i Results4_7 = crp_add_4_32(ResultVector4_7, NumbersVector4_7);
        crp_store_unaligned_128(&Result[0], Results0_3);
        crp_store_unaligned_128(&Result[1], Results4_7);
    # endif
}

[[ nodiscard ]]
std::uint32_t* raw_sha256(const void* Data, std::uint64_t Length) noexcept
{
    constexpr static std::uint32_t Initial[] = {
        0x6A09E667, 0xBB67AE85,
        0x3C6EF372, 0xA54FF53A,
        0x510E527F, 0x9B05688C,
        0x1F83D9AB, 0x5BE0CD19
    };
    std::uint64_t Iter = 0x0;
    const std::uint64_t ReservedLength = 64 * ((Length + 8) / 64 + 1);
    
    # ifndef CRP_USE_SIMD
        std::uint32_t* Result = (std::uint32_t*)crp_allocate(32);
        std::memcpy(Result, Initial, 32);

        std::uint8_t* MessageBlock = (std::uint8_t*)crp_allocate(ReservedLength);

        std::memset(MessageBlock, 0x0, ReservedLength);
        std::memcpy(MessageBlock, static_cast<const std::uint8_t*>(Data), Length);
        MessageBlock[Length] = (std::uint8_t)0b10000000;
        
        Length *= 8;
        Iter = sizeof Length;
        while(Iter--)
            MessageBlock[ReservedLength - Iter - 1] = (std::uint8_t)(Length >> (8 * Iter));
        
        for(Iter = 0x0; Iter < ReservedLength; Iter += 64)
            update(Result, MessageBlock + Iter);

        crp_free(MessageBlock);
        return Result;
    # else
        crp_align(crp_wordsize) static crp_128i Result[2];

        for(; Iter < 2; Iter += 1)
            Result[Iter] = crp_set_4_32 (
                static_cast<std::int32_t>(Initial[Iter * 4 + 3]),
                static_cast<std::int32_t>(Initial[Iter * 4 + 2]),
                static_cast<std::int32_t>(Initial[Iter * 4 + 1]),
                static_cast<std::int32_t>(Initial[Iter * 4])
            );

        std::uint8_t* MessageBlock = (std::uint8_t*)crp_allocate(ReservedLength);
        const static crp_128i ZeroVector = crp_zero_128();
        
        # pragma omp simd safelen(4)
        for(Iter = 0x0; Iter < ReservedLength; Iter += 16)
            crp_store_aligned_128((crp_128i*)&MessageBlock[Iter], ZeroVector);

        # pragma omp simd
        for(Iter = 0x0; Iter < Length; Iter += 1)
            MessageBlock[Iter] = static_cast<const std::uint8_t*>(Data)[Iter];

        MessageBlock[Length] = (std::uint8_t)0b10000000;
        
        Length *= 8;
        Iter = sizeof Length;
        while(Iter--)
            MessageBlock[ReservedLength - Iter - 1] = (std::uint8_t)(Length >> (8 * Iter));
        
        for(Iter = 0x0; Iter < ReservedLength; Iter += 64)
            update(Result, MessageBlock + Iter);

        std::uint32_t* __Result = (std::uint32_t*)crp_allocate(32);
        crp_store_unaligned_128((crp_128i*)&__Result[0], Result[0]);
        crp_store_unaligned_128((crp_128i*)&__Result[4], Result[1]);
        crp_free(MessageBlock);
        return __Result;
    # endif
}

static std::string symbol_to_hex(const std::uint8_t x, const CRP_FORMAT Format)
{
    std::string String;
    String.resize(3);

    switch(Format) {
        case CRP_FORMAT::upper:
            std::snprintf(String.data(), 3, "%02X", x);
            break;
        case CRP_FORMAT::lower:
            std::snprintf(String.data(), 3, "%02x", x);
            break;
    }
    return String;
}

std::string crp_to_hex(const std::uint32_t* HexData, std::uint64_t HexLength, const std::string& Delimiter, const CRP_FORMAT Format) noexcept
{
    std::string Result;
    Result.reserve(288);

    if(Delimiter.empty())
        for(std::uint64_t Iter = 0x0; Iter < HexLength; Iter += 1)
        {
            Result += symbol_to_hex((HexData[Iter] >> 24) & 255, Format);
            Result += symbol_to_hex((HexData[Iter] >> 16) & 255, Format);
            Result += symbol_to_hex((HexData[Iter] >> 8) & 255, Format);
            Result += symbol_to_hex(HexData[Iter] & 255, Format);
        }
    else
    {
        for(std::uint64_t Iter = 0x0; Iter < HexLength; Iter += 1)
        {
            Result += symbol_to_hex((HexData[Iter] >> 24) & 255, Format);
            Result += Delimiter;
            Result += symbol_to_hex((HexData[Iter] >> 16) & 255, Format);
            Result += Delimiter;
            Result += symbol_to_hex((HexData[Iter] >> 8) & 255, Format);
            Result += Delimiter;
            Result += symbol_to_hex(HexData[Iter] & 255, Format);
            Result += Delimiter;
        }
        Result.pop_back();
    }
    return Result;
}

std::string sha256(const void* Data, const std::uint64_t Length, const std::string Delimiter, const CRP_FORMAT Format) noexcept
{
    std::uint32_t* __Result = raw_sha256(Data, Length);
    std::string Result = crp_to_hex(__Result, CRP_SHA256_32BYTES_BLOCKS_COUNT, Delimiter, Format);

    crp_free(__Result);
    return Result;
}