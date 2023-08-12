# include <benchmark/benchmark.h>

# include <crp/sha256.hpp>
# include <cryptopp/sha.h>
# include <cryptopp/hex.h>
# include <cryptopp/files.h>
# include <cryptopp/cryptlib.h>

# include <openssl/sha.h>

# include <sstream>
# include <iomanip>

const char* str = "";
const int Length = std::strlen(str);

static void BENCH_CRP_SHA256(benchmark::State& state)
{
    for(auto _ : state)
    {
        std::string Result = sha256(str, Length);
    }
}

static void BENCH_CRYPTOPP_SHA256(benchmark::State& state)
{
    CryptoPP::SHA256 hash;
    for(auto _ : state)
    {
        std::string Result;
        std::ostringstream oss;
        hash.Update((const CryptoPP::byte*)str, Length);
        Result.resize(hash.DigestSize());
        hash.Final((CryptoPP::byte*)Result.data());
        CryptoPP::HexEncoder encoder(new CryptoPP::FileSink(oss));
        CryptoPP::StringSource(Result, true, new CryptoPP::Redirector(encoder));
        Result = oss.str();
    }
}

static void BENCH_CRP_RAW_SHA256(benchmark::State& state)
{
    for(auto _ : state)
    {
        crp_free(raw_sha256(str, Length));
    }
}

static void BENCH_CRYPTOPP_RAW_SHA256(benchmark::State& state)
{
    CryptoPP::SHA256 hash;
    for(auto _ : state)
    {
        std::uint8_t* Result = (std::uint8_t*)crp_allocate(256);
        hash.Update((const CryptoPP::byte*)str, Length);
        hash.Final((CryptoPP::byte*)Result);
        crp_free(Result);
    }
}

static void BENCH_OPENSSL_RAW_SHA256(benchmark::State& state)
{
    SHA256_CTX sha256;
    SHA256_Init(&sha256);

    for(auto _ : state)
    {
        std::uint8_t* Result = (std::uint8_t*)crp_allocate(256);
        SHA256_Update(&sha256, str, Length);
        SHA256_Final(Result, &sha256);
        crp_free(Result);
        
    }
}

static void BENCH_OPENSSL_SHA256(benchmark::State& state)
{
    CryptoPP::SHA256 hash;
    for(auto _ : state)
    {
        std::string Result;
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        SHA256_Update(&sha256, str, Length);
        SHA256_Final(hash, &sha256);
        std::stringstream ss;
        for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        {
            ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
        }
        Result.resize(SHA256_DIGEST_LENGTH);
        Result = ss.str();
    }
}

BENCHMARK(BENCH_CRP_SHA256);
BENCHMARK(BENCH_CRP_RAW_SHA256);
BENCHMARK(BENCH_CRYPTOPP_SHA256);
BENCHMARK(BENCH_CRYPTOPP_RAW_SHA256);
BENCHMARK(BENCH_OPENSSL_SHA256);
BENCHMARK(BENCH_OPENSSL_RAW_SHA256);
BENCHMARK_MAIN();