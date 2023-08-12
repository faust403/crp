# include <benchmark/benchmark.h>
# include <crp/sha256.hpp>

static void BENCH_SHA256(benchmark::State& state)
{
    const char* str = "Hello, world!";
    const int Length = std::strlen(str);
    for(auto _ : state)
    {
        raw_sha256(str, Length);
    }
}
// Register the function as a benchmark
BENCHMARK(BENCH_SHA256);

BENCHMARK_MAIN();