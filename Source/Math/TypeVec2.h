#pragma once

#include "Common.h"
#include "TypeVec.h"
#ifdef MATH_ENABLE_SWIZZLE
#include "Swizzle.h"
#endif

namespace math {

	template<Numeric T>
	struct vec<2, T> {
		using Type = vec<2, T>;
		using DataType = T;
		static constexpr usize Length = 2;

		static const vec<2, T> Up;
		static const vec<2, T> Down;
		static const vec<2, T> Right;
		static const vec<2, T> Left;

		constexpr vec() = default;
		constexpr explicit vec(T s) : x(s), y(s) { }
		constexpr vec(T x, T y) : x(x), y(y) { }
		constexpr vec(const vec<2, T>& other)
			: x(other.x), y(other.y) { }

		template<Numeric U>
		constexpr explicit vec(const vec<2, U>& other)
			: x(static_cast<T>(other.x)),
			y(static_cast<T>(other.y)) { }

#ifdef MATH_ENABLE_SWIZZLE
		template<i32 E0, i32 E1>
		constexpr vec(const detail::Swizzle<T, 2, E0, E1, -1, -1>& other)
			: x(other().x), y(other().y) { }

		template<Numeric U, i32 E0, i32 E1>
		constexpr explicit vec(const detail::Swizzle<U, 2, E0, E1, -1, -1>& other)
			: x(static_cast<T>(other().x)),
			y(static_cast<T>(other().y)) { }
#endif

		constexpr inline T& operator[](usize i) {
			ASSERT(i >= 0 && i < 2, "vec2 subscript out of range!");
			if (i == 0) return x;
			return y;
		}

		constexpr inline const T& operator[](usize i) const {
			ASSERT(i >= 0 && i < 2, "vec2 subscript out of range!");
			if (i == 0) return x;
			return y;
		}

		constexpr inline bool operator==(const vec<2, T>& other) const {
			return x == other.x && y == other.y;
		}

		constexpr inline bool operator!=(const vec<2, T>& other) const {
			return x != other.x || y != other.y;
		}

		constexpr inline void operator+=(const vec<2, T>& other) {
			x += other.x;
			y += other.y;
		}

		constexpr inline void operator-=(const vec<2, T>& other) {
			x -= other.x;
			y -= other.y;
		}

		constexpr inline void operator*=(const vec<2, T>& other) {
			x *= other.x;
			y *= other.y;
		}

		constexpr inline void operator/=(const vec<2, T>& other) {
			x /= other.x;
			y /= other.y;
		}

#ifdef MATH_ENABLE_SWIZZLE
		template<i32 E0, i32 E1>
		constexpr inline bool operator==(const detail::Swizzle<T, 2, E0, E1, -1, -1>& other) const {
			return x == other().x && y == other().y;
		}

		template<i32 E0, i32 E1>
		constexpr inline bool operator!=(const detail::Swizzle<T, 2, E0, E1, -1, -1>& other) const {
			return x != other().x || y != other().y;
		}

		template<i32 E0, i32 E1>
		constexpr inline void operator+=(const detail::Swizzle<T, 2, E0, E1, -1, -1>& other) {
			x += other().x;
			y += other().y;
		}

		template<i32 E0, i32 E1>
		constexpr inline void operator-=(const detail::Swizzle<T, 2, E0, E1, -1, -1>& other) {
			x -= other().x;
			y -= other().y;
		}

		template<i32 E0, i32 E1>
		constexpr inline void operator*=(const detail::Swizzle<T, 2, E0, E1, -1, -1>& other) {
			x *= other().x;
			y *= other().y;
		}

		template<i32 E0, i32 E1>
		constexpr inline void operator/=(const detail::Swizzle<T, 2, E0, E1, -1, -1>& other) {
			x /= other().x;
			y /= other().y;
		}
#endif

		constexpr inline void operator*=(T s) {
			x *= s;
			y *= s;
		}

		constexpr inline void operator/=(T s) {
			const typename types::IntToFloat<T>::Type invS = 1.0 / s;
			x *= invS;
			y *= invS;
		}

		constexpr inline vec<2, T> operator-() const { return vec<2, T>(-x, -y); }
		constexpr inline vec<2, T> operator+() const { return *this; }

		union {
			struct { T x, y; };
			struct { T r, g; };
			struct { T u, v; };
			struct { T s, t; };

#ifdef MATH_ENABLE_SWIZZLE
			detail::Swizzle<T, 2, 0, 0, -1, -1> xx, rr, uu, ss;
			detail::Swizzle<T, 2, 1, 0, -1, -1> yx, gr, vu, ts;
			detail::Swizzle<T, 2, 0, 1, -1, -1> xy, rg, uv, st;
			detail::Swizzle<T, 2, 1, 1, -1, -1> yy, gg, vv, tt;

			detail::Swizzle<T, 3, 0, 0, 0, -1> xxx, rrr, uuu, sss;
			detail::Swizzle<T, 3, 1, 0, 0, -1> yxx, grr, vuu, tss;
			detail::Swizzle<T, 3, 0, 1, 0, -1> xyx, rgr, uvu, sts;
			detail::Swizzle<T, 3, 1, 1, 0, -1> yyx, ggr, vvu, tts;
			detail::Swizzle<T, 3, 0, 0, 1, -1> xxy, rrg, uuv, sst;
			detail::Swizzle<T, 3, 1, 0, 1, -1> yxy, grg, vuv, tst;
			detail::Swizzle<T, 3, 0, 1, 1, -1> xyy, rgg, uvv, stt;
			detail::Swizzle<T, 3, 1, 1, 1, -1> yyy, ggg, vvv, ttt;

			detail::Swizzle<T, 4, 0, 0, 0, 0> xxxx, rrrr, uuuu, ssss;
			detail::Swizzle<T, 4, 1, 0, 0, 0> yxxx, grrr, vuuu, tsss;
			detail::Swizzle<T, 4, 0, 1, 0, 0> xyxx, rgrr, uvuu, stss;
			detail::Swizzle<T, 4, 1, 1, 0, 0> yyxx, ggrr, vvuu, ttss;
			detail::Swizzle<T, 4, 0, 0, 1, 0> xxyx, rrgr, uuvu, ssts;
			detail::Swizzle<T, 4, 1, 0, 1, 0> yxyx, grgr, vuvu, tsts;
			detail::Swizzle<T, 4, 0, 1, 1, 0> xyyx, rggr, uvvu, stts;
			detail::Swizzle<T, 4, 1, 1, 1, 0> yyyx, gggr, vvvu, ttts;
			detail::Swizzle<T, 4, 0, 0, 0, 1> xxxy, rrrg, uuuv, ssst;
			detail::Swizzle<T, 4, 1, 0, 0, 1> yxxy, grrg, vuuv, tsst;
			detail::Swizzle<T, 4, 0, 1, 0, 1> xyxy, rgrg, uvuv, stst;
			detail::Swizzle<T, 4, 1, 1, 0, 1> yyxy, ggrg, vvuv, ttst;
			detail::Swizzle<T, 4, 0, 0, 1, 1> xxyy, rrgg, uuvv, sstt;
			detail::Swizzle<T, 4, 1, 0, 1, 1> yxyy, grgg, vuvv, tstt;
			detail::Swizzle<T, 4, 0, 1, 1, 1> xyyy, rggg, uvvv, sttt;
			detail::Swizzle<T, 4, 1, 1, 1, 1> yyyy, gggg, vvvv, tttt;
#endif
		};
	};

	template<Numeric T>
	const vec<2, T> vec<2, T>::Up = vec<2, T>(0, 1);

	template<Numeric T>
	const vec<2, T> vec<2, T>::Down = vec<2, T>(0, -1);

	template<Numeric T>
	const vec<2, T> vec<2, T>::Right = vec<2, T>(1, 0);

	template<Numeric T>
	const vec<2, T> vec<2, T>::Left = vec<2, T>(-1, 0);

	//Common Functions
	template<Numeric T>
	constexpr inline T Dot(const vec<2, T>& a, const vec<2, T>& b) {
		return a.x * b.x + a.y * b.y;
	}

	template<Numeric T>
	constexpr inline vec<2, T> Abs(const vec<2, T>& v) {
		return vec<2, T>(Abs(v.x), Abs(v.y));
	}

	template<Numeric T>
	constexpr inline vec<2, T> Min(const vec<2, T>& a, const vec<2, T>& b) {
		return vec<2, T>(Min(a.x, b.x), Min(a.y, b.y));
	}

	template<Numeric T>
	constexpr inline vec<2, T> Max(const vec<2, T>& a, const vec<2, T>& b) {
		return vec<2, T>(Max(a.x, b.x), Max(a.y, b.y));
	}

	template<Floating T>
	constexpr inline vec<2, T> Floor(const vec<2, T>& v) {
		return vec<2, T>(Floor(v.x), Floor(v.y));
	}

	template<Floating T>
	constexpr inline vec<2, T> Ceil(const vec<2, T>& v) {
		return vec<2, T>(Ceil(v.x), Ceil(v.y));
	}

	typedef vec<2, f32>  vec2;
	typedef vec<2, f64> dvec2;
	typedef vec<2, i32> ivec2;
	typedef vec<2, u32> uvec2;
	typedef vec<2, b8>  bvec2;
}

template<math::Numeric T>
constexpr math::vec<2, T> operator+(const math::vec<2, T>& a, const math::vec<2, T>& b) {
	return math::vec<2, T>(a.x + b.x, a.y + b.y);
}

template<math::Numeric T>
constexpr math::vec<2, T> operator-(const math::vec<2, T>& a, const math::vec<2, T>& b) {
	return math::vec<2, T>(a.x - b.x, a.y - b.y);
}

template<math::Numeric T>
constexpr math::vec<2, T> operator*(const math::vec<2, T>& a, const math::vec<2, T>& b) {
	return math::vec<2, T>(a.x * b.x, a.y * b.y);
}

template<math::Numeric T>
constexpr math::vec<2, T> operator/(const math::vec<2, T>& a, const math::vec<2, T>& b) {
	return math::vec<2, T>(a.x / b.x, a.y / b.y);
}

template<math::Numeric T>
constexpr math::vec<2, T> operator%(const math::vec<2, T>& a, const math::vec<2, T>& b) {
	return math::vec<2, T>(a.x % b.x, a.y % b.y);
}

template<math::Numeric T>
constexpr math::vec<2, T> operator*(const math::vec<2, T>& v, T s) {
	return math::vec<2, T>(v.x * s, v.y * s);
}

template<math::Numeric T>
constexpr math::vec<2, T> operator/(const math::vec<2, T>& v, T s) {
	const typename types::IntToFloat<T>::Type invS = 1.0 / s;
	return math::vec<2, T>(v.x * invS, v.y * invS);
}