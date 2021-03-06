#ifndef CHROMA_NUMERIC_HALF_H
#define CHROMA_NUMERIC_HALF_H

#include <stdint.h>
#include <limits>
#include <type_traits>
#include "details/compiler.h"

namespace numeric {

/*
* half-float
*
*  1   5       10
* +-+------+------------+
* |s|eee.ee|mm.mmmm.mmmm|
* +-+------+------------+
*
* minimum (denormal) value: 2^-24 = 5.96e-8
* minimum (normal) value:   2^-14 = 6.10e-5
* maximum value:            2-2^-10 = 65504
*
* Integers between 0 and 2048 can be represented exactly
*/
#ifdef __ARM_NEON

using half = __fp16;

inline constexpr  
uint16_t getbits(half const& h) noexcept {
    return MAKE_CONSTEXPR(reinterpret_cast<uint16_t const&>(h));
}

inline constexpr 
half makehalf(uint16_t bits) noexcept {
    return MAKE_CONSTEXPR(reinterpret_cast<half const&>(bits));
}

#else

class NUM_EMPTY_BASES half {
    struct fp16 {
        uint16_t bits;
        fp16() noexcept = default;
        explicit constexpr fp16(uint16_t bits) noexcept : bits(bits) {}

        constexpr void set_s(unsigned int s) noexcept { bits = uint16_t((bits & 0x7FFF) | (s << 15)); }
        constexpr void set_e(unsigned int s) noexcept { bits = uint16_t((bits & 0xE3FF) | (s << 10)); }
        constexpr void set_m(unsigned int s) noexcept { bits = uint16_t((bits & 0xFC00) | (s << 0)); }
        constexpr unsigned int get_s() const noexcept { return  bits >> 15u; }
        constexpr unsigned int get_e() const noexcept { return (bits >> 10u) & 0x1Fu; }
        constexpr unsigned int get_m() const noexcept { return  bits & 0x3FFu; }
    };
    struct fp32 {
        union {
            uint32_t bits = 0;
            float fp;
        };
        constexpr fp32() noexcept {}
        explicit constexpr fp32(float f) noexcept : fp(f) {}
        explicit constexpr fp32(uint32_t b) noexcept : bits(b) {}

        constexpr void set_s(unsigned int s) noexcept { bits = uint32_t((bits & 0x7FFFFFFF) | (s << 31)); }
        constexpr void set_e(unsigned int s) noexcept { bits = uint32_t((bits & 0x807FFFFF) | (s << 23)); }
        constexpr void set_m(unsigned int s) noexcept { bits = uint32_t((bits & 0xFF800000) | (s << 0)); }
        constexpr unsigned int get_s() const noexcept { return  bits >> 31u; }
        constexpr unsigned int get_e() const noexcept { return (bits >> 23u) & 0xFFu; }
        constexpr unsigned int get_m() const noexcept { return  bits & 0x7FFFFFu; }
    };

public:
    half() = default;
    constexpr half(float v) noexcept : m_bits(ftoh(v)) {}
    constexpr operator float() const noexcept { return htof(m_bits); }

private:
    // these are friends, not members (and they're not "private")
    friend constexpr uint16_t getbits(half const& h) noexcept { return h.m_bits.bits; }
    friend constexpr inline half makehalf(uint16_t bits) noexcept;

    enum Binary { binary };
    explicit constexpr half(Binary, uint16_t bits) noexcept : m_bits(bits) { }

    static inline constexpr fp16 ftoh(float v) noexcept;
    static inline constexpr float htof(fp16 v) noexcept;

private:
    fp16 m_bits;
};

constexpr inline half makehalf(uint16_t bits) noexcept {
    return half(numeric::half::binary, bits);
}

constexpr half::fp16 half::ftoh(float f) noexcept {
    constexpr fp32 infinity(31u << 23);
    constexpr fp32 magic(15u << 23);
    fp32 in(f);
    fp16 out(0);
    unsigned int sign = in.get_s();

    in.set_s(0);
    if (NUM_UNLIKELY(in.get_e() == 0xFF)) { // inf or nan
        out.set_e(0x1F);
        out.set_m(in.get_m() ? 0x200 : 0);
    }
    else {
        in.bits &= ~0xFFF;
        in.fp *= magic.fp;
        in.bits += 0x1000;
        in.bits = in.bits < infinity.bits ? in.bits : infinity.bits;
        out.bits = uint16_t(in.bits >> 13);
    }
    out.set_s(sign);
    return out;
}

constexpr float half::htof(half::fp16 in) noexcept {
    constexpr fp32 magic((0xFEu - 0xFu) << 23);
    constexpr fp32 infnan(0x8Fu << 23);
    fp32 out((in.bits & 0x7FFFu) << 13);
    out.fp *= magic.fp;
    if (out.fp >= infnan.fp) {
        out.bits |= 0xFFu << 23;
    }
    out.bits |= (in.bits & 0x8000u) << 16;
    return out.fp;
}

#endif // __ARM_NEON

inline constexpr numeric::half operator"" _h(long double v) {
    return numeric::half(static_cast<float>(v));
}

} // namespace numeric

namespace std {

template<> struct is_floating_point<numeric::half> 
    : public std::true_type 
{};

template<>
class numeric_limits<numeric::half> {
public:
    typedef numeric::half type;

    static constexpr const bool is_specialized = true;
    static constexpr const bool is_signed = true;
    static constexpr const bool is_integer = false;
    static constexpr const bool is_exact = false;
    static constexpr const bool has_infinity = true;
    static constexpr const bool has_quiet_NaN = true;
    static constexpr const bool has_signaling_NaN = false;
    static constexpr const float_denorm_style has_denorm = denorm_absent;
    static constexpr const bool has_denorm_loss = true;
    static constexpr const bool is_iec559 = false;
    static constexpr const bool is_bounded = true;
    static constexpr const bool is_modulo = false;
    static constexpr const bool traps = false;
    static constexpr const bool tinyness_before = false;
    static constexpr const float_round_style round_style = round_indeterminate;

    static constexpr const int digits = 11;
    static constexpr const int digits10 = 3;
    static constexpr const int max_digits10 = 5;
    static constexpr const int radix = 2;
    static constexpr const int min_exponent = -13;
    static constexpr const int min_exponent10 = -4;
    static constexpr const int max_exponent = 16;
    static constexpr const int max_exponent10 = 4;

    inline static constexpr type round_error() noexcept { return numeric::makehalf(0x3800); }
    inline static constexpr type min() noexcept { return numeric::makehalf(0x0400); }
    inline static constexpr type max() noexcept { return numeric::makehalf(0x7bff); }
    inline static constexpr type lowest() noexcept { return numeric::makehalf(0xfbff); }
    inline static constexpr type epsilon() noexcept { return numeric::makehalf(0x1400); }
    inline static constexpr type infinity() noexcept { return numeric::makehalf(0x7c00); }
    inline static constexpr type quiet_NaN() noexcept { return numeric::makehalf(0x7fff); }
    inline static constexpr type denorm_min() noexcept { return numeric::makehalf(0x0001); }
    inline static constexpr type signaling_NaN() noexcept { return numeric::makehalf(0x7dff); }
};

} // namespace std

#endif