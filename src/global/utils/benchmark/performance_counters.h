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
        counters = profiler.read_counters();
        if (profiler.is_valid()) {
            state.counters.insert({
                    { "C",   { (double)counters.get_cpu_cycles(),    benchmark::Counter::kAvgIterations }},
                    { "I",   { (double)counters.get_instructions(), benchmark::Counter::kAvgIterations }},
                    { "BPU", { (double)counters.get_branch_misses(), benchmark::Counter::kAvgIterations }},
                    { "CPI", { (double)counters.get_cpi(),          benchmark::Counter::kAvgThreads }},
            });
        }
    }
};

#endif //TNT_UTILS_BENCHMARK_PEROFRMANCECOUNTERS_H
