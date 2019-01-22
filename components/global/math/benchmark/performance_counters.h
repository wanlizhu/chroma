#ifndef CHROMA_MATH_BENCHMARK_PEROFRMANCECOUNTERS_H
#define CHROMA_MATH_BENCHMARK_PEROFRMANCECOUNTERS_H

#include <benchmark/benchmark.h>
#include <utils/profiler.h>
#include <cmath>

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

    void stop() {
        profiler.stop();
    }

    ~PerformanceCounters() {
        profiler.stop();
        counters = profiler.read_counters();
        if (profiler.is_valid()) {
            auto avgItem = double(state.iterations()) / state.items_processed();
            state.counters.insert({
                    { "C",   { avgItem * (double)counters.get_cpu_cycles(),    benchmark::Counter::kAvgIterations }},
                    { "I",   { avgItem * (double)counters.get_instructions(), benchmark::Counter::kAvgIterations }},
                    { "BPU", { std::floor(0.5 + avgItem * (double)counters.get_branch_misses() / state.iterations()), benchmark::Counter::kDefaults }},
                    { "CPI", {           (double)counters.get_cpi(),          benchmark::Counter::kAvgThreads }},
            });
        }
    }
};

#endif 
