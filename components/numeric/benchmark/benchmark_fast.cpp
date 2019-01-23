#include "performance_counters.h"
#include <benchmark/benchmark.h>
#include <numeric/fast.h>
#include <numeric/half.h>
#include <cmath>

using namespace numeric;


struct Scalar{};
struct Vector{};


template <typename A, typename T>
static void BM_trig(benchmark::State& state) noexcept {
    T f;
    state.SetLabel(T::label());
    std::vector<float> data(1024);
    std::vector<typename T::result_type> res(1024);
    for (size_t i = 0; i < data.size(); i++) {
        data[i] = float((float(i) / data.size()) * M_2_PI - M_PI);
    }

    {
        PerformanceCounters pc(state);
        for (auto _ : state) {
            if (std::is_same<A, Vector>::value) {
//#pragma clang loop vectorize(enable)
                for (size_t i = 0, c = data.size(); i < c; i++) {
                    res[i] = f(data[i]);
                }
            } else if (std::is_same<A, Scalar>::value) {
//#pragma clang loop unroll(disable) vectorize(disable)
                for (size_t i = 0, c = data.size(); i < c; i++) {
                    res[i] = f(data[i]);
                }
            }
            benchmark::ClobberMemory();
            benchmark::DoNotOptimize(res);
        }
        pc.stop();
        state.SetItemsProcessed(state.iterations() * data.size());
    }
}

UTILS_NOINLINE
static void init(std::vector<float>& v) noexcept {
    for (size_t i = 0; i < v.size(); i++) {
        v[i] = (float(i + 1) / (v.size() + 1)) * 1024;
    }
}

template <typename A, typename T>
static void BM_func(benchmark::State& state) noexcept {
    T f;
    state.SetLabel(T::label());
    std::vector<typename T::result_type> res(1024);
    std::vector<float> data(1024);
    init(data);

    {
        PerformanceCounters pc(state);
        for (auto _ : state) {
            if (std::is_same<A, Vector>::value) {
//#pragma clang loop vectorize(enable)
                for (size_t i = 0, c = data.size(); i < c; i++) {
                    res[i] = f(data[i]);
                }
            } else if (std::is_same<A, Scalar>::value) {
//#pragma clang loop unroll(disable) vectorize(disable)
                for (size_t i = 0, c = data.size(); i < c; i++) {
                    res[i] = f(data[i]);
                }
            }
            benchmark::ClobberMemory();
            benchmark::DoNotOptimize(res);
        }
        pc.stop();
        state.SetItemsProcessed(state.iterations() * data.size());
    }
}

struct StdCos {
    using result_type = float;
    float operator()(float v) { return std::cos(v); }
    static const char* label() { return "std::cos"; }
};
struct FastCos {
    using result_type = float;
    float operator()(float v) { return fast::cos(v); }
    static const char* label() { return "fast::cos"; }
};
struct StdLog2 {
    using result_type = float;
    float operator()(float v) { return std::log2(v); }
    static const char* label() { return "std::log2"; }
};
struct FastLog2 {
    using result_type = float;
    float operator()(float v) { return fast::log2(v); }
    static const char* label() { return "fast::log2"; }
};
struct Rcp {
    using result_type = float;
    float operator()(float v) { return 1.0f/v; }
    static const char* label() { return "1/x"; }
};
struct StdISqrt {
    using result_type = float;
    float operator()(float v) { return 1.0f / std::sqrt(v); }
    static const char* label() { return "1/std::sqrt"; }
};
struct FastISqrt {
    using result_type = float;
    float operator()(float v) { return fast::isqrt(v); }
    static const char* label() { return "fast::isqrt"; }
};
struct StdPow2dot2 {
    using result_type = float;
    float operator()(float v) { return std::pow(v, 2.2f); }
    static const char* label() { return "std::pow(x, 2.2f)"; }
};
struct StdExp2dot2Log {
    using result_type = float;
    float operator()(float v) { return std::exp(2.2f * std::log(v)); }
    static const char* label() { return "std::exp(2.2f * std::log(x))"; }
};
struct FastPow2dot2 {
    using result_type = float;
    float operator()(float v) { return fast::pow2dot2(v); }
    static const char* label() { return "fast::pow2dot2"; }
};

struct Float16 {
    using result_type = numeric::half;
    numeric::half operator()(float v) { return numeric::half(v); }
    static const char* label() { return "half"; }
};


BENCHMARK_TEMPLATE(BM_trig, Scalar, StdCos);
BENCHMARK_TEMPLATE(BM_trig, Scalar, FastCos);
BENCHMARK_TEMPLATE(BM_trig, Vector, FastCos);

BENCHMARK_TEMPLATE(BM_func, Scalar, StdLog2);
BENCHMARK_TEMPLATE(BM_func, Scalar, FastLog2);
BENCHMARK_TEMPLATE(BM_func, Vector, FastLog2);

BENCHMARK_TEMPLATE(BM_func, Scalar, Rcp);
BENCHMARK_TEMPLATE(BM_func, Vector, Rcp);

BENCHMARK_TEMPLATE(BM_func, Scalar, StdISqrt);
BENCHMARK_TEMPLATE(BM_func, Vector, StdISqrt);
BENCHMARK_TEMPLATE(BM_func, Scalar, FastISqrt);
BENCHMARK_TEMPLATE(BM_func, Vector, FastISqrt);

BENCHMARK_TEMPLATE(BM_func, Scalar, StdPow2dot2);
BENCHMARK_TEMPLATE(BM_func, Scalar, StdExp2dot2Log);
BENCHMARK_TEMPLATE(BM_func, Scalar, FastPow2dot2);
BENCHMARK_TEMPLATE(BM_func, Vector, FastPow2dot2);

BENCHMARK_TEMPLATE(BM_func, Scalar, Float16);
BENCHMARK_TEMPLATE(BM_func, Vector, Float16);
