#ifndef CHROMA_UTILS_BENCHMARK_PEROFRMANCECOUNTERS_H
#define CHROMA_UTILS_BENCHMARK_PEROFRMANCECOUNTERS_H

#include <benchmark/benchmark.h>

#include <utils/Profiler.h>

class PerformanceCounters {
    benchmark::State& state;
    utils::Profiler profiler;
    utils::Profiler::Counters counters{};

public:
    explicit PerformanceCounters(benchmark::State& state)
            : state(state) {
        profiler.reset_events(utils::Profiler::EV_CPU_CYCLES | utils::Profiler::EV_BPU_MISSES);
        profiler.start();
    }
    ~PerformanceCounters() {
        profiler.stop();
        counters = profiler.readCounters();
        if (profiler.is_valid()) {
            state.counters.insert({
                    { "C",   { (double)counters.getCpuCycles(),    benchmark::Counter::kAvgIterations }},
                    { "I",   { (double)counters.getInstructions(), benchmark::Counter::kAvgIterations }},
                    { "BPU", { (double)counters.getBranchMisses(), benchmark::Counter::kAvgIterations }},
                    { "CPI", { (double)counters.getCPI(),          benchmark::Counter::kAvgThreads }},
            });
        }
    }
};

#endif //TNT_UTILS_BENCHMARK_PEROFRMANCECOUNTERS_H
