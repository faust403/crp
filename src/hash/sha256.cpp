// https://helix.stormhub.org/papers/SHA-256.pdf

# include <crp/sha256.hpp>

# define ch(x, y, z) (((x) & (y)) ^ (~(x) & (z)))

# define maj(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ (y & (z)))

// interpreting into ROR
# define ROTR(x, r) (0x0 | (x >> r) | (x << (8 * sizeof(x) - r)))

# define S0(x) (ROTR((x), 2) ^ ROTR((x), 13) ^ ROTR((x), 22))

# define S1(x) (ROTR((x), 6) ^ ROTR((x), 11) ^ ROTR((x), 25))

# define s0(x) (ROTR((x), 7) ^ ROTR((x), 18) ^ (x >> 3))

# define s1(x) (ROTR((x), 17) ^ ROTR((x), 19) ^ (x >> 10))

# define Round0_15(i) \
    Schedule[i] = static_cast<std::uint32_t>(From[i]) << 24 | static_cast<std::uint32_t>(From[i + 1]) << 16 | static_cast<std::uint32_t>(From[i + 2]) << 8 | static_cast<std::uint32_t>(From[i + 3]); \
    tmp1 = h + S1(e) + ch(e, f, g) + Constants[i] + Schedule[i]; \
    tmp2 = S0(a) + maj(a, b, c); \
    h = g; g = f; f = e; \
    e = d + tmp1; d = c; \
    c = b; b = a; \
    a = tmp1 + tmp2; \

# define Round16_63(i) \
    Schedule[i] = s1(Schedule[i - 2]) + Schedule[i - 7] + s0(Schedule[i - 15]) + Schedule[i - 16]; \
    tmp1 = h + S1(e) + ch(e, f, g) + Constants[i] + Schedule[i]; \
    tmp2 = S0(a) + maj(a, b, c); \
    h = g; g = f; f = e; \
    e = d + tmp1; d = c; \
    c = b; b = a; \
    a = tmp1 + tmp2; \

# define Concat4_8_32(target, a, b, c, d) \
    target = static_cast<decltype(target)>(a) << 24 \
    | static_cast<decltype(target)>(b) << 16 \
    | static_cast<decltype(target)>(c) << 8 \
    | static_cast<decltype(target)>(d); \

constexpr static std::uint32_t Initial[] = {
    0x6A09E667, 0xBB67AE85,
    0x3C6EF372, 0xA54FF53A,
    0x510E527F, 0x9B05688C,
    0x1F83D9AB, 0x5BE0CD19
};

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

template<typename Type>
[[ nodiscard ]] std::string to_binary(Type Data) noexcept
{
    std::string Result = "";

    std::uint64_t CurrentBit = 8 * sizeof Data;
    while(CurrentBit--)
        Result += ((Data >> CurrentBit) & 1) == 0 ? "0" : "1";

    return Result;
}

static void update(std::uint32_t* Result, const std::uint8_t* From) noexcept
{
    static std::uint32_t Schedule[64]; 

    std::uint32_t a = Result[0];
    std::uint32_t b = Result[1];
    std::uint32_t c = Result[2];
    std::uint32_t d = Result[3];
    std::uint32_t e = Result[4];
    std::uint32_t f = Result[5];
    std::uint32_t g = Result[6];
    std::uint32_t h = Result[7];

    std::uint32_t tmp1;
    std::uint32_t tmp2;

    Round0_15(0); Round0_15(1); Round0_15(2); Round0_15(3);
    Round0_15(4); Round0_15(5); Round0_15(6); Round0_15(7);
    Round0_15(8); Round0_15(9); Round0_15(10); Round0_15(11);
    Round0_15(12); Round0_15(13); Round0_15(14); Round0_15(15);
    Round16_63(16); Round16_63(17); Round16_63(18); Round16_63(19);
    Round16_63(20); Round16_63(21); Round16_63(22); Round16_63(23);
    Round16_63(24); Round16_63(25); Round16_63(26); Round16_63(27);
    Round16_63(28); Round16_63(29); Round16_63(30); Round16_63(31);
    Round16_63(32); Round16_63(33); Round16_63(34); Round16_63(35);
    Round16_63(36); Round16_63(37); Round16_63(38); Round16_63(39);
    Round16_63(40); Round16_63(41); Round16_63(42); Round16_63(43);
    Round16_63(44); Round16_63(45); Round16_63(46); Round16_63(47);
    Round16_63(48); Round16_63(49); Round16_63(50); Round16_63(51);
    Round16_63(52); Round16_63(53); Round16_63(54); Round16_63(55);
    Round16_63(56); Round16_63(57); Round16_63(58); Round16_63(59);
    Round16_63(60); Round16_63(61); Round16_63(62); Round16_63(63);

    Result[0] += a;
    Result[1] += b;
    Result[2] += c;
    Result[3] += d;
    Result[4] += e;
    Result[5] += f;
    Result[6] += g;
    Result[7] += h;
}

[[ nodiscard ]]
std::uint32_t* raw_sha256(const void* Data, std::uint64_t Length) noexcept
{
    std::uint64_t Iter = 0x0;
    const std::uint64_t ReservedLength = 64 * ((Length + 8) / 64 + 1);
    
    std::uint32_t* Result = static_cast<std::uint32_t*>(crp_allocate(32));
    std::uint8_t* MessageBlock = static_cast<std::uint8_t*>(alloca(ReservedLength));

    Result[0] = Initial[0];
    Result[1] = Initial[1];
    Result[2] = Initial[2];
    Result[3] = Initial[3];
    Result[4] = Initial[4];
    Result[5] = Initial[5];
    Result[6] = Initial[6];
    Result[7] = Initial[7];

    std::memset(MessageBlock + Length, 0x0, ReservedLength);
    std::memcpy(MessageBlock, static_cast<const std::uint8_t*>(Data), Length);
    MessageBlock[Length] = (std::uint8_t)0b10000000;
    
    Length *= 8;
    MessageBlock[ReservedLength - 1] = 255 & (Length);
    MessageBlock[ReservedLength - 2] = 255 & (Length >> 8);
    MessageBlock[ReservedLength - 3] = 255 & (Length >> 16);
    MessageBlock[ReservedLength - 4] = 255 & (Length >> 24);
    MessageBlock[ReservedLength - 5] = 255 & (Length >> 32);
    MessageBlock[ReservedLength - 6] = 255 & (Length >> 40);
    MessageBlock[ReservedLength - 7] = 255 & (Length >> 48);
    MessageBlock[ReservedLength - 8] = 255 & (Length >> 56);

    for(Iter = 0x0; Iter < ReservedLength; Iter += 64)
        update(Result, MessageBlock + Iter);

    return Result;
}

[[ nodiscard ]]
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

[[ nodiscard ]]
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

[[ nodiscard ]]
std::string sha256(const void* Data, const std::uint64_t Length, const std::string Delimiter, const CRP_FORMAT Format) noexcept
{
    std::uint32_t* __Result = raw_sha256(Data, Length);
    const std::string Result = crp_to_hex(__Result, CRP_SHA256_32BYTES_BLOCKS_COUNT, Delimiter, Format);

    crp_free(__Result);
    return Result;
}