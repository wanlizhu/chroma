#ifndef CHROMA_UTILS_PROFILER_H
#define CHROMA_UTILS_PROFILER_H

#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <chrono>

#if defined(__linux__)
#   include <unistd.h>
#   include <sys/ioctl.h>
#   include <linux/perf_event.h>
#endif

#include <utils/compiler.h>

namespace utils {

class Profiler {
public:
    enum {
        INSTRUCTIONS    = 0,   // must be zero
        CPU_CYCLES      = 1,
        DCACHE_REFS     = 2,
        DCACHE_MISSES   = 3,
        BRANCHES        = 4,
        BRANCH_MISSES   = 5,
        ICACHE_REFS     = 6,
        ICACHE_MISSES   = 7,
        EVENT_COUNT
    };

    enum {
        EV_CPU_CYCLES = 1 << CPU_CYCLES,
        EV_L1D_REFS   = 1 << DCACHE_REFS,
        EV_L1D_MISSES = 1 << DCACHE_MISSES,
        EV_BPU_REFS   = 1 << BRANCHES,
        EV_BPU_MISSES = 1 << BRANCH_MISSES,
        EV_L1I_REFS   = 1 << ICACHE_REFS,
        EV_L1I_MISSES = 1 << ICACHE_MISSES,

        EV_L1D_RATES = EV_L1D_REFS | EV_L1D_MISSES,
        EV_L1I_RATES = EV_L1I_REFS | EV_L1I_MISSES,
        EV_BPU_RATES = EV_BPU_REFS | EV_BPU_MISSES,
    };

    Profiler() noexcept; // must call reset_events()
    explicit Profiler(uint32_t eventMask) noexcept;
    ~Profiler() noexcept;

    Profiler(const Profiler& rhs) = delete;
    Profiler(Profiler&& rhs) = delete;
    Profiler& operator=(const Profiler& rhs) = delete;
    Profiler& operator=(Profiler&& rhs) = delete;

    // selects which events are enabled. 
    uint32_t reset_events(uint32_t eventMask) noexcept;
    uint32_t enabled_events() const noexcept { return m_enabled_events; }

    // could return false if performance counters are not supported/enabled
    bool is_valid() const { return m_counters_fd[0] >= 0; }

    class Counters {
        friend class Profiler;
        uint64_t nr;
        uint64_t time_enabled;
        uint64_t time_running;
        struct {
            uint64_t value;
            uint64_t id;
        } counters[Profiler::EVENT_COUNT];

        friend Counters operator-(Counters lhs, const Counters& rhs) noexcept {
            lhs.nr -= rhs.nr;
            lhs.time_enabled -= rhs.time_enabled;
            lhs.time_running -= rhs.time_running;
            for (size_t i = 0; i < EVENT_COUNT; ++i) {
                lhs.counters[i].value -= rhs.counters[i].value;
            }
            return lhs;
        }

    public:
        uint64_t get_instructions() const { return counters[INSTRUCTIONS].value; }
        uint64_t get_cpu_cycles() const { return counters[CPU_CYCLES].value; }
        uint64_t get_l1d_references() const { return counters[DCACHE_REFS].value; }
        uint64_t get_l1d_misses() const { return counters[DCACHE_MISSES].value; }
        uint64_t get_l1i_references() const { return counters[ICACHE_REFS].value; }
        uint64_t get_l1i_misses() const { return counters[ICACHE_MISSES].value; }
        uint64_t get_branch_instructions() const { return counters[BRANCHES].value; }
        uint64_t get_branch_misses() const { return counters[BRANCH_MISSES].value; }

        std::chrono::duration<uint64_t, std::nano> getWallTime() const {
            return std::chrono::duration<uint64_t, std::nano>(time_enabled);
        }

        std::chrono::duration<uint64_t, std::nano> getRunningTime() const {
            return std::chrono::duration<uint64_t, std::nano>(time_running);
        }

        double getIPC() const noexcept {
            uint64_t cpuCycles = get_cpu_cycles();
            uint64_t instructions = get_instructions();
            return double(instructions) / double(cpuCycles);
        }

        double getCPI() const noexcept {
            uint64_t cpuCycles = get_cpu_cycles();
            uint64_t instructions = get_instructions();
            return double(cpuCycles) / double(instructions);
        }

        double getL1DMissRate() const noexcept {
            uint64_t cacheReferences = get_l1d_references();
            uint64_t cacheMisses = get_l1d_misses();
            return double(cacheMisses) / double(cacheReferences);
        }

        double getL1DHitRate() const noexcept {
            return 1.0 - getL1DMissRate();
        }

        double getL1IMissRate() const noexcept {
            uint64_t cacheReferences = get_l1i_references();
            uint64_t cacheMisses = get_l1i_misses();
            return double(cacheMisses) / double(cacheReferences);
        }

        double getL1IHitRate() const noexcept {
            return 1.0 - getL1IMissRate();
        }

        double getBranchMissRate() const noexcept {
            uint64_t branchReferences = get_branch_instructions();
            uint64_t branchMisses = get_branch_misses();
            return double(branchMisses) / double(branchReferences);
        }

        double getBranchHitRate() const noexcept {
            return 1.0 - getBranchMissRate();
        }

        double getMPKI(uint64_t misses) const noexcept {
            return (misses * 1000.0) / get_instructions();
        }
    };

#if defined(__linux__)

    void reset() noexcept {
        int fd = m_counters_fd[0];
        ioctl(fd, PERF_EVENT_IOC_RESET,  PERF_IOC_FLAG_GROUP);
    }

    void start() noexcept {
        int fd = m_counters_fd[0];
        ioctl(fd, PERF_EVENT_IOC_ENABLE, PERF_IOC_FLAG_GROUP);
    }

    void stop() noexcept {
        int fd = m_counters_fd[0];
        ioctl(fd, PERF_EVENT_IOC_DISABLE, PERF_IOC_FLAG_GROUP);
    }

    Counters read_counters() noexcept;

#else // !__linux__

    void reset() noexcept {}
    void start() noexcept {}
    void stop() noexcept {}
    Counters read_counters() noexcept { return {}; }

#endif // __linux__

    bool has_branch_rates() const noexcept {
        return (m_counters_fd[BRANCHES] >= 0) && (m_counters_fd[BRANCH_MISSES] >= 0);
    }

    bool has_icache_rates() const noexcept {
        return (m_counters_fd[ICACHE_REFS] >= 0) && (m_counters_fd[ICACHE_MISSES] >= 0);
    }

private:
    UTILS_UNUSED uint8_t m_ids[EVENT_COUNT] = {};
    int m_counters_fd[EVENT_COUNT];
    uint32_t m_enabled_events = 0;
};

} // namespace utils

#endif 
