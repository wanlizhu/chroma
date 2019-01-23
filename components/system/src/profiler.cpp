#include <system/profiler.h>
#include <stdlib.h>
#include <string.h>
#if !defined(WIN32)
#   include <unistd.h>
#else
#   include <io.h>
#   define close _close
#endif

#include <algorithm>
#include <memory>

#if defined(__linux__)

#include <sys/syscall.h>

#ifdef __ARM_ARCH
    enum ARMv8PmuPerfTypes{
        // Common micro-architecture events
        ARMV8_PMUV3_PERFCTR_L1_ICACHE_REFILL    = 0x01,
        ARMV8_PMUV3_PERFCTR_L1_ICACHE_ACCESS    = 0x14,
        ARMV8_PMUV3_PERFCTR_L2_CACHE_ACCESS     = 0x16,
        ARMV8_PMUV3_PERFCTR_L2_CACHE_REFILL     = 0x17,
        ARMV8_PMUV3_PERFCTR_L2_CACHE_WB         = 0x18,
    };
#endif

static int perf_event_open(perf_event_attr* hw_event, pid_t pid,
        int cpu, int group_fd, unsigned long flags) {
    return (int)syscall(__NR_perf_event_open, hw_event, pid, cpu, group_fd, flags);
}

#endif // __linux__

namespace sys {

Profiler::Profiler() noexcept {
    std::uninitialized_fill(std::begin(m_counters_fd), std::end(m_counters_fd), -1);
}

Profiler::Profiler(uint32_t eventMask) noexcept : Profiler() {
    Profiler::reset_events(eventMask);
}

Profiler::~Profiler() noexcept {
    #pragma nounroll
    for (int fd : m_counters_fd) {
        if (fd >= 0) {
            close(fd);
        }
    }
}

uint32_t Profiler::reset_events(uint32_t eventMask) noexcept {
    // close all counters
    #pragma nounroll
    for (int& fd : m_counters_fd) {
        if (fd >= 0) {
            close(fd);
            fd = -1;
        }
    }
    m_enabled_events = 0;

#if defined(__linux__)

    perf_event_attr pe{};
    pe.type = PERF_TYPE_HARDWARE;
    pe.size = sizeof(perf_event_attr);
    pe.config = PERF_COUNT_HW_INSTRUCTIONS;
    pe.disabled = 1;
    pe.exclude_kernel = 1;
    pe.exclude_hv = 1;
    pe.read_format = PERF_FORMAT_GROUP |
                     PERF_FORMAT_ID |
                     PERF_FORMAT_TOTAL_TIME_ENABLED |
                     PERF_FORMAT_TOTAL_TIME_RUNNING;

    uint8_t count = 0;
    int fd = perf_event_open(&pe, 0, -1, -1, 0);
    if (fd >= 0) {
        const int groupFd = fd;
        m_ids[INSTRUCTIONS] = count++;
        m_counters_fd[INSTRUCTIONS] = fd;

        pe.read_format = PERF_FORMAT_GROUP | PERF_FORMAT_ID;

        if (eventMask & EV_CPU_CYCLES) {
            pe.type = PERF_TYPE_HARDWARE;
            pe.config = PERF_COUNT_HW_CPU_CYCLES;
            m_counters_fd[CPU_CYCLES] = perf_event_open(&pe, 0, -1, groupFd, 0);
            if (m_counters_fd[CPU_CYCLES] > 0) {
                m_ids[CPU_CYCLES] = count++;
                m_enabled_events |= EV_CPU_CYCLES;
            }
        }

        if (eventMask & EV_L1D_REFS) {
            pe.type = PERF_TYPE_HARDWARE;
            pe.config = PERF_COUNT_HW_CACHE_REFERENCES;
            m_counters_fd[DCACHE_REFS] = perf_event_open(&pe, 0, -1, groupFd, 0);
            if (m_counters_fd[DCACHE_REFS] > 0) {
                m_ids[DCACHE_REFS] = count++;
                m_enabled_events |= EV_L1D_REFS;
            }
        }

        if (eventMask & EV_L1D_MISSES) {
            pe.type = PERF_TYPE_HARDWARE;
            pe.config = PERF_COUNT_HW_CACHE_MISSES;
            m_counters_fd[DCACHE_MISSES] = perf_event_open(&pe, 0, -1, groupFd, 0);
            if (m_counters_fd[DCACHE_MISSES] > 0) {
                m_ids[DCACHE_MISSES] = count++;
                m_enabled_events |= EV_L1D_MISSES;
            }
        }

        if (eventMask & EV_BPU_REFS) {
            pe.type = PERF_TYPE_HARDWARE;
            pe.config = PERF_COUNT_HW_BRANCH_INSTRUCTIONS;
            m_counters_fd[BRANCHES] = perf_event_open(&pe, 0, -1, groupFd, 0);
            if (m_counters_fd[BRANCHES] > 0) {
                m_ids[BRANCHES] = count++;
                m_enabled_events |= EV_BPU_REFS;
            }
        }

        if (eventMask & EV_BPU_MISSES) {
            pe.type = PERF_TYPE_HARDWARE;
            pe.config = PERF_COUNT_HW_BRANCH_MISSES;
            m_counters_fd[BRANCH_MISSES] = perf_event_open(&pe, 0, -1, groupFd, 0);
            if (m_counters_fd[BRANCH_MISSES] > 0) {
                m_ids[BRANCH_MISSES] = count++;
                m_enabled_events |= EV_BPU_MISSES;
            }
        }

#ifdef __ARM_ARCH
        if (eventMask & EV_L1I_REFS) {
            pe.type = PERF_TYPE_RAW;
            pe.config = ARMV8_PMUV3_PERFCTR_L1_ICACHE_ACCESS;
            m_counters_fd[ICACHE_REFS] = perf_event_open(&pe, 0, -1, groupFd, 0);
            if (m_counters_fd[ICACHE_REFS] > 0) {
                m_ids[ICACHE_REFS] = count++;
                m_enabled_events |= EV_L1I_REFS;
            }
        }

        if (eventMask & EV_L1I_MISSES) {
            pe.type = PERF_TYPE_RAW;
            pe.config = ARMV8_PMUV3_PERFCTR_L1_ICACHE_REFILL;
            m_counters_fd[ICACHE_MISSES] = perf_event_open(&pe, 0, -1, groupFd, 0);
            if (m_counters_fd[ICACHE_MISSES] > 0) {
                m_ids[ICACHE_MISSES] = count++;
                m_enabled_events |= EV_L1I_MISSES;
            }
        }
#else
        if (eventMask & EV_L1I_REFS) {
            pe.type = PERF_TYPE_HW_CACHE;
            pe.config = PERF_COUNT_HW_CACHE_L1I | 
                (PERF_COUNT_HW_CACHE_OP_READ<<8) | (PERF_COUNT_HW_CACHE_RESULT_ACCESS<<16);
            m_counters_fd[ICACHE_REFS] = perf_event_open(&pe, 0, -1, groupFd, 0);
            if (m_counters_fd[ICACHE_REFS] > 0) {
                m_ids[ICACHE_REFS] = count++;
                m_enabled_events |= EV_L1I_REFS;
            }
        }

        if (eventMask & EV_L1I_MISSES) {
            pe.type = PERF_TYPE_HW_CACHE;
            pe.config = PERF_COUNT_HW_CACHE_L1I | 
                (PERF_COUNT_HW_CACHE_OP_READ<<8) | (PERF_COUNT_HW_CACHE_RESULT_MISS<<16);
            m_counters_fd[ICACHE_MISSES] = perf_event_open(&pe, 0, -1, groupFd, 0);
            if (m_counters_fd[ICACHE_MISSES] > 0) {
                m_ids[ICACHE_MISSES] = count++;
                m_enabled_events |= EV_L1I_MISSES;
            }
        }
#endif
    }
#endif // __linux__
    return m_enabled_events;
}

#if defined(__linux__)

Profiler::Counters Profiler::read_counters() noexcept {
    Counters outCounters{};
    Counters counters; // NOLINT
    ssize_t n = read(m_counters_fd[0], &counters, sizeof(Counters));
    if (n > 0) {
        outCounters.nr = counters.nr;
        outCounters.time_enabled = counters.time_enabled;
        outCounters.time_running = counters.time_running;
        for (size_t i = 0; i < size_t(EVENT_COUNT); i++) {
            // in theory we should check that m_counters_fd[i] >= 0, but we don't to avoid
            // a branch, m_ids[] is initialized such we won't access past the counters array.
            outCounters.counters[i] = counters.counters[m_ids[i]];
        }
    }
    return outCounters;
}

#endif // __linux__

} // namespace sys
