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


static void update (std::uint32_t* Result, const std::uint8_t* From) noexcept
{
    static std::uint32_t Schedule[64]; 

    Schedule[0] = static_cast<std::uint32_t>(From[0]) << 24 | static_cast<std::uint32_t>(From[1]) << 16 | static_cast<std::uint32_t>(From[2]) << 8 | static_cast<std::uint32_t>(From[3]);
    Schedule[1] = static_cast<std::uint32_t>(From[4]) << 24 | static_cast<std::uint32_t>(From[5]) << 16 | static_cast<std::uint32_t>(From[6]) << 8 | static_cast<std::uint32_t>(From[7]);
    Schedule[2] = static_cast<std::uint32_t>(From[8]) << 24 | static_cast<std::uint32_t>(From[9]) << 16 | static_cast<std::uint32_t>(From[10]) << 8 | static_cast<std::uint32_t>(From[11]);
    Schedule[3] = static_cast<std::uint32_t>(From[12]) << 24 | static_cast<std::uint32_t>(From[13]) << 16 | static_cast<std::uint32_t>(From[14]) << 8 | static_cast<std::uint32_t>(From[15]);
    Schedule[4] = static_cast<std::uint32_t>(From[16]) << 24 | static_cast<std::uint32_t>(From[17]) << 16 | static_cast<std::uint32_t>(From[18]) << 8 | static_cast<std::uint32_t>(From[19]);
    Schedule[5] = static_cast<std::uint32_t>(From[20]) << 24 | static_cast<std::uint32_t>(From[21]) << 16 | static_cast<std::uint32_t>(From[22]) << 8 | static_cast<std::uint32_t>(From[23]);
    Schedule[6] = static_cast<std::uint32_t>(From[24]) << 24 | static_cast<std::uint32_t>(From[25]) << 16 | static_cast<std::uint32_t>(From[26]) << 8 | static_cast<std::uint32_t>(From[27]);
    Schedule[7] = static_cast<std::uint32_t>(From[28]) << 24 | static_cast<std::uint32_t>(From[29]) << 16 | static_cast<std::uint32_t>(From[30]) << 8 | static_cast<std::uint32_t>(From[31]);
    Schedule[8] = static_cast<std::uint32_t>(From[32]) << 24 | static_cast<std::uint32_t>(From[33]) << 16 | static_cast<std::uint32_t>(From[34]) << 8 | static_cast<std::uint32_t>(From[35]);
    Schedule[9] = static_cast<std::uint32_t>(From[36]) << 24 | static_cast<std::uint32_t>(From[37]) << 16 | static_cast<std::uint32_t>(From[38]) << 8 | static_cast<std::uint32_t>(From[39]);
    Schedule[10] = static_cast<std::uint32_t>(From[40]) << 24 | static_cast<std::uint32_t>(From[41]) << 16 | static_cast<std::uint32_t>(From[42]) << 8 | static_cast<std::uint32_t>(From[43]);
    Schedule[11] = static_cast<std::uint32_t>(From[44]) << 24 | static_cast<std::uint32_t>(From[45]) << 16 | static_cast<std::uint32_t>(From[46]) << 8 | static_cast<std::uint32_t>(From[47]);
    Schedule[12] = static_cast<std::uint32_t>(From[48]) << 24 | static_cast<std::uint32_t>(From[49]) << 16 | static_cast<std::uint32_t>(From[50]) << 8 | static_cast<std::uint32_t>(From[51]);
    Schedule[13] = static_cast<std::uint32_t>(From[52]) << 24 | static_cast<std::uint32_t>(From[53]) << 16 | static_cast<std::uint32_t>(From[54]) << 8 | static_cast<std::uint32_t>(From[55]);
    Schedule[14] = static_cast<std::uint32_t>(From[56]) << 24 | static_cast<std::uint32_t>(From[57]) << 16 | static_cast<std::uint32_t>(From[58]) << 8 | static_cast<std::uint32_t>(From[59]);
    Schedule[15] = static_cast<std::uint32_t>(From[60]) << 24 | static_cast<std::uint32_t>(From[61]) << 16 | static_cast<std::uint32_t>(From[62]) << 8 | static_cast<std::uint32_t>(From[63]);
    Schedule[16] = s1(Schedule[14]) + Schedule[9] + s0(Schedule[1]) + Schedule[0];
    Schedule[17] = s1(Schedule[15]) + Schedule[10] + s0(Schedule[2]) + Schedule[1];
    Schedule[18] = s1(Schedule[16]) + Schedule[11] + s0(Schedule[3]) + Schedule[2];
    Schedule[19] = s1(Schedule[17]) + Schedule[12] + s0(Schedule[4]) + Schedule[3];
    Schedule[20] = s1(Schedule[18]) + Schedule[13] + s0(Schedule[5]) + Schedule[4];
    Schedule[21] = s1(Schedule[19]) + Schedule[14] + s0(Schedule[6]) + Schedule[5];
    Schedule[22] = s1(Schedule[20]) + Schedule[15] + s0(Schedule[7]) + Schedule[6];
    Schedule[23] = s1(Schedule[21]) + Schedule[16] + s0(Schedule[8]) + Schedule[7];
    Schedule[24] = s1(Schedule[22]) + Schedule[17] + s0(Schedule[9]) + Schedule[8];
    Schedule[25] = s1(Schedule[23]) + Schedule[18] + s0(Schedule[10]) + Schedule[9];
    Schedule[26] = s1(Schedule[24]) + Schedule[19] + s0(Schedule[11]) + Schedule[10];
    Schedule[27] = s1(Schedule[25]) + Schedule[20] + s0(Schedule[12]) + Schedule[11];
    Schedule[28] = s1(Schedule[26]) + Schedule[21] + s0(Schedule[13]) + Schedule[12];
    Schedule[29] = s1(Schedule[27]) + Schedule[22] + s0(Schedule[14]) + Schedule[13];
    Schedule[30] = s1(Schedule[28]) + Schedule[23] + s0(Schedule[15]) + Schedule[14];
    Schedule[31] = s1(Schedule[29]) + Schedule[24] + s0(Schedule[16]) + Schedule[15];
    Schedule[32] = s1(Schedule[30]) + Schedule[25] + s0(Schedule[17]) + Schedule[16];
    Schedule[33] = s1(Schedule[31]) + Schedule[26] + s0(Schedule[18]) + Schedule[17];
    Schedule[34] = s1(Schedule[32]) + Schedule[27] + s0(Schedule[19]) + Schedule[18];
    Schedule[35] = s1(Schedule[33]) + Schedule[28] + s0(Schedule[20]) + Schedule[19];
    Schedule[36] = s1(Schedule[34]) + Schedule[29] + s0(Schedule[21]) + Schedule[20];
    Schedule[37] = s1(Schedule[35]) + Schedule[30] + s0(Schedule[22]) + Schedule[21];
    Schedule[38] = s1(Schedule[36]) + Schedule[31] + s0(Schedule[23]) + Schedule[22];
    Schedule[39] = s1(Schedule[37]) + Schedule[32] + s0(Schedule[24]) + Schedule[23];
    Schedule[40] = s1(Schedule[38]) + Schedule[33] + s0(Schedule[25]) + Schedule[24];
    Schedule[41] = s1(Schedule[39]) + Schedule[34] + s0(Schedule[26]) + Schedule[25];
    Schedule[42] = s1(Schedule[40]) + Schedule[35] + s0(Schedule[27]) + Schedule[26];
    Schedule[43] = s1(Schedule[41]) + Schedule[36] + s0(Schedule[28]) + Schedule[27];
    Schedule[44] = s1(Schedule[42]) + Schedule[37] + s0(Schedule[29]) + Schedule[28];
    Schedule[45] = s1(Schedule[43]) + Schedule[38] + s0(Schedule[30]) + Schedule[29];
    Schedule[46] = s1(Schedule[44]) + Schedule[39] + s0(Schedule[31]) + Schedule[30];
    Schedule[47] = s1(Schedule[45]) + Schedule[40] + s0(Schedule[32]) + Schedule[31];
    Schedule[48] = s1(Schedule[46]) + Schedule[41] + s0(Schedule[33]) + Schedule[32];
    Schedule[49] = s1(Schedule[47]) + Schedule[42] + s0(Schedule[34]) + Schedule[33];
    Schedule[50] = s1(Schedule[48]) + Schedule[43] + s0(Schedule[35]) + Schedule[34];
    Schedule[51] = s1(Schedule[49]) + Schedule[44] + s0(Schedule[36]) + Schedule[35];
    Schedule[52] = s1(Schedule[50]) + Schedule[45] + s0(Schedule[37]) + Schedule[36];
    Schedule[53] = s1(Schedule[51]) + Schedule[46] + s0(Schedule[38]) + Schedule[37];
    Schedule[54] = s1(Schedule[52]) + Schedule[47] + s0(Schedule[39]) + Schedule[38];
    Schedule[55] = s1(Schedule[53]) + Schedule[48] + s0(Schedule[40]) + Schedule[39];
    Schedule[56] = s1(Schedule[54]) + Schedule[49] + s0(Schedule[41]) + Schedule[40];
    Schedule[57] = s1(Schedule[55]) + Schedule[50] + s0(Schedule[42]) + Schedule[41];
    Schedule[58] = s1(Schedule[56]) + Schedule[51] + s0(Schedule[43]) + Schedule[42];
    Schedule[59] = s1(Schedule[57]) + Schedule[52] + s0(Schedule[44]) + Schedule[43];
    Schedule[60] = s1(Schedule[58]) + Schedule[53] + s0(Schedule[45]) + Schedule[44];
    Schedule[61] = s1(Schedule[59]) + Schedule[54] + s0(Schedule[46]) + Schedule[45];
    Schedule[62] = s1(Schedule[60]) + Schedule[55] + s0(Schedule[47]) + Schedule[46];
    Schedule[63] = s1(Schedule[61]) + Schedule[56] + s0(Schedule[48]) + Schedule[47];

    std::uint32_t a = Result[0];
    std::uint32_t b = Result[1];
    std::uint32_t c = Result[2];
    std::uint32_t d = Result[3];
    std::uint32_t e = Result[4];
    std::uint32_t f = Result[5];
    std::uint32_t g = Result[6];
    std::uint32_t h = Result[7];

    for(std::uint8_t Iter = 0x0; Iter < 64; Iter += 1)
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
}

[[ nodiscard ]] std::uint32_t* raw_sha256(const void* Data, std::uint64_t Length) noexcept
{
    std::uint64_t Iter = 0x0;
    const std::uint64_t ReservedLength = 64 * ((Length + 8) / 64 + 1);
    
    std::uint32_t* Result = (std::uint32_t*)crp_allocate(32);
    Result[0] = Initial[0];
    Result[1] = Initial[1];
    Result[2] = Initial[2];
    Result[3] = Initial[3];
    Result[4] = Initial[4];
    Result[5] = Initial[5];
    Result[6] = Initial[6];
    Result[7] = Initial[7];

    std::uint8_t* MessageBlock = (std::uint8_t*)alloca(ReservedLength);
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