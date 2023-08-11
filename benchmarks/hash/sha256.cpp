# include <benchmark/benchmark.h>
# include <crp/sha256.hpp>

static void BENCH_SHA256(benchmark::State& state)
{
    for(auto _ : state)
    {
        const char* str = "Hello, world!";
        sha256(str, std::strlen(str));
    }
}
// Register the function as a benchmark
BENCHMARK(BENCH_SHA256);

BENCHMARK_MAIN();