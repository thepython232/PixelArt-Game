#pragma once

#include "Util\Types.h"
#include "Util\Std.h"
#undef min
#undef max

namespace math {
	template<typename T>
	concept Integral = types::AnySame<T, i8, u8, i16, u16, i32, u32, i64, u64, usize, isize,
#ifdef GAME_HAS_INT128
		i128, u128,
#endif
#ifdef __cpp_char8_t
		char8_t,
#endif
		b8, char, wchar_t, char16_t, char32_t, size_t>;

	template<typename T>
	concept Floating = types::AnySame<T, f32, f64
#ifdef GAME_HAS_FLOAT128
		, f128
#endif
	>;

	template<typename T>
	concept Numeric = Integral<T> || Floating<T>;

	template<typename T>
	concept Signed = Integral<T> && static_cast<T>(-1) < static_cast<T>(0);

	template<typename T>
	concept Unsigned = !Signed<T>;

	template<Floating T>
	consteval T Pi() {
		return T(3.141592653589793);
	}

	constexpr f64 EPSILON = std::numeric_limits<f64>::epsilon();
	constexpr f32 MAX_FLOAT = std::numeric_limits<f32>::max();
	constexpr f64 MAX_DOUBLE = std::numeric_limits<f64>::max();
	constexpr f64 INFINITY = std::numeric_limits<f64>::infinity();
	constexpr f64 NEGATIVE_INFINITY = -std::numeric_limits<f64>::infinity();
	constexpr f64 TO_DEGREES = 180.0 / Pi<f64>();
	constexpr f64 TO_RADIANS = Pi<f64>() / 180.0;

	template<Floating T>
	consteval T Tau() {
		return T(6.283185307179586);
	}

	template<Floating T>
	consteval T TwoPi() {
		return T(6.283185307179586);
	}

	template<Floating T>
	consteval T PiOverTwo() {
		return T(1.570796326794896);
	}

	template<Floating T>
	consteval T PiOverFour() {
		return T(0.785398163397448);
	}

	template<Floating T>
	consteval T E() {
		return T(2.718281828459045);
	}

	template<Floating T>
	consteval T RootTwo() {
		return T(1.414213562373095);
	}

	template<Numeric T>
	constexpr inline T Min(T a, T b) {
		return (a < b) ? a : b;
	}

	template<Numeric T>
	constexpr inline T Max(T a, T b) {
		return (a > b) ? a : b;
	}

	template<Numeric T>
	constexpr inline T Clamp(T t, T min, T max) {
		return Max(Min(t, max), min);
	}

	template<Floating T>
	constexpr inline T Floor(T t) {
		return static_cast<T>((u64)t);
	}

	template<Floating T>
	constexpr inline T Ceil(T t) {
		return static_cast<T>((u64)(t - EPSILON)) + 1.0;
	}

	template<Floating T>
	constexpr inline T Fract(T t) {
		return t - Floor(t);
	}

	template<Floating T>
	constexpr inline T Mod(T a, T b) {
		return fmodf(a, b);
	}

	template<Integral T>
	constexpr inline T Mod(T a, T b) {
		return a % b;
	}

	template<Numeric T>
	inline T Radians(T degs) {
		return degs * TO_RADIANS;
	}

	template<Numeric T>
	inline T Degrees(T rads) {
		return rads * TO_DEGREES;
	}

	template<Floating T>
	inline T Sin(T t) {
#ifdef MATH_USE_DEGREES
		return std::sin(t * TO_RADIANS);
#else
		return std::sin(t);
#endif
	}

	template<Floating T>
	inline T Cos(T t) {
#ifdef MATH_USE_DEGREES
		return std::cos(t * TO_RADIANS);
#else
		return std::cos(t);
#endif
	}

	template<Floating T>
	inline T Tan(T t) {
#ifdef MATH_USE_DEGREES
		return std::tan(t * TO_RADIANS);
#else
		return std::tan(t);
#endif
	}

	template<Floating T>
	inline T Csc(T t) {
		return (T)1.0 / Sin(t);
	}

	template<Floating T>
	inline T Sec(T t) {
		return (T)1.0 / Cos(t);
	}

	template<Floating T>
	inline T Cot(T t) {
		return (T)1.0 / Tan(t);
	}

	template<Floating T>
	inline T ASin(T t) {
#ifdef MATH_USE_DEGREES
		return std::asin(t) * TO_DEGREES;
#else
		return std::asin(t);
#endif
	}

	template<Floating T>
	inline T ACos(T t) {
#ifdef MATH_USE_DEGREES
		return std::acos(t) * TO_DEGREES;
#else
		return std::acos(t);
#endif
	}

	template<Floating T>
	inline T ATan(T t) {
#ifdef MATH_USE_DEGREES
		return std::atan(t) * TO_DEGREES;
#else
		return std::atan(t);
#endif
	}

	template<Floating T>
	inline T ATan2(T x, T y) {
#ifdef MATH_USE_DEGREES
		return std::atan2(x, y) * TO_DEGREES;
#else
		return std::atan2(x, y);
#endif
	}

	template<Numeric T>
	inline T Sqrt(T t) {
		return std::sqrt(t);
	}

	template<Numeric T>
	inline T Log(T t) {
		return std::log(t);
	}

	template<Numeric T>
	inline T Log2(T t) {
		return std::log2(t);
	}

	template<Numeric T>
	inline T LogBase(T t, T base) {
		return Log(t) / Log(base);
	}

	template<Numeric T>
	inline T Pow(T base, T power) {
		return std::pow(base, power);
	}
}