#include <benchmark/benchmark.h>
#include <string_view>

// https://www.bfilipek.com/2018/07/string-view-perf.html
// http://quick-bench.com/F1NGrjNtcNimqG2q6QzvHKPDpQY

static void StringSubStr(benchmark::State &state)
{
    std::string s = "Hello Super Extra Programming World";
    for (auto _ : state)
    {
        auto oneStr = s.substr(0, 5);
        auto twoStr = s.substr(6, 5);
        auto threeStr = s.substr(12, 5);
        auto fourStr = s.substr(18, 11);
        auto fiveStr = s.substr(30, 5);
        // Make sure the variable is not optimized away by compiler
        benchmark::DoNotOptimize(oneStr);
        benchmark::DoNotOptimize(twoStr);
        benchmark::DoNotOptimize(threeStr);
        benchmark::DoNotOptimize(fourStr);
        benchmark::DoNotOptimize(fiveStr);
    }
}
// Register the function as a benchmark
BENCHMARK(StringSubStr);

static void StringViewSubStr(benchmark::State &state)
{
    // Code before the loop is not measured
    std::string s = "Hello Super Extra Programming World";
    for (auto _ : state)
    {
        std::string_view sv = s;
        auto oneSv = sv.substr(0, 5);
        auto twoSv = sv.substr(6, 5);
        auto threeSv = sv.substr(12, 5);
        auto fourSv = sv.substr(18, 11);
        auto fiveSv = sv.substr(30, 5);

        benchmark::DoNotOptimize(oneSv);
        benchmark::DoNotOptimize(twoSv);
        benchmark::DoNotOptimize(threeSv);
        benchmark::DoNotOptimize(fourSv);
        benchmark::DoNotOptimize(fiveSv);
    }
}
BENCHMARK(StringViewSubStr);

BENCHMARK_MAIN();
