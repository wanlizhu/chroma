#include "performance_counters.h"
#include <benchmark/benchmark.h>


static void BM_memcpy(benchmark::State& state) {
    char* src = new char[state.range(0)];
    char* dst = new char[state.range(0)];
    memset(src, 'x', (size_t)state.range(0));

    {
        PerformanceCounters pc(state);
        for (auto _ : state) {
            memcpy(dst, src, (size_t)state.range(0));
            benchmark::DoNotOptimize(dst);
            benchmark::DoNotOptimize(src);
            benchmark::ClobberMemory();
        }
    }
    state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(state.range(0)));

    delete[] src;
    delete[] dst;
}

BENCHMARK(BM_memcpy)->Range(8, 8192<<10)->Threads(1)->Threads(8);
