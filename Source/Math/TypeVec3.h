#pragma once

#include "Common.h"
#include "TypeVec.h"
#ifdef MATH_ENABLE_SWIZZLE
#include "Swizzle.h"
#endif

namespace math {

	template<Numeric T>
	struct vec<3, T> {
		using Type = vec<3, T>;
		using DataType = T;
		static constexpr usize Length = 3;

		static const vec<3, T> Up;
		static const vec<3, T> Down;
		static const vec<3, T> Right;
		static const vec<3, T> Left;
		static const vec<3, T> Forward;
		static const vec<3, T> Backward;

		constexpr vec() = default;
		constexpr explicit vec(T s)
			: x(s), y(s), z(s) { }
		constexpr vec(T x, T y, T z)
			: x(x), y(y), z(z) { }

		template<Numeric U>
		constexpr explicit vec(const vec<3, U>& other)
			: x(static_cast<T>(other.x)),
			y(static_cast<T>(other.y)),
			z(static_cast<T>(other.z)) { }

#ifdef MATH_ENABLE_SWIZZLE
		template<i32 E0, i32 E1, i32 E2>
		constexpr vec(const detail::Swizzle<T, 3, E0, E1, E2, -1>& other)
			: x(other().x), y(other().y), z(other().z) { }

		template<Numeric U, i32 E0, i32 E1, i32 E2>
		constexpr explicit vec(const detail::Swizzle<U, 3, E0, E1, E2, -1>& other)
			: x(static_cast<T>(other().x)),
			x(static_cast<T>(other().x)),
			x(static_cast<T>(other().x)) { }
#endif

		constexpr explicit vec(const vec<2, T>& a, T b)
			: x(a.x), y(a.y), z(b) { }

		constexpr explicit vec(T a, const vec<2, T>& b)
			: x(a), y(b.x), z(b.y) { }

		template<Numeric U, Numeric V>
		constexpr explicit vec(const vec<2, U>& a, V b)
			: x(static_cast<T>(a.x)),
			y(static_cast<T>(a.y)),
			z(static_cast<T>(b)) { }

		template<Numeric U, Numeric V>
		constexpr explicit vec(U a, const vec<2, V>& b)
			: x(static_cast<T>(a)),
			y(static_cast<T>(b.x)),
			z(static_cast<T>(b.y)) { }

#ifdef MATH_ENABLE_SWIZZLE
		template<i32 E0, i32 E1>
		constexpr explicit vec(const detail::Swizzle<T, 2, E0, E1, -1, -1>& a, T b)
			: x(a().x), y(a().y), z(b) { }

		template<i32 E0, i32 E1>
		constexpr explicit vec(T a, const detail::Swizzle<T, 2, E0, E1, -1, -1>& b)
			: x(a), y(b().x), z(b().y) { }

		template<Numeric U, Numeric V, i32 E0, i32 E1>
		constexpr explicit vec(const detail::Swizzle<T, 2, E0, E1, -1, -1>& a, V b)
			: x(static_cast<T>(a().x)),
			y(static_cast<T>(a().y)),
			z(static_cast<T>(b)) { }

		template<Numeric U, Numeric V, i32 E0, i32 E1>
		constexpr explicit vec(U a, const detail::Swizzle<T, 2, E0, E1, -1, -1>& b)
			: x(static_cast<T>(a)),
			y(static_cast<T>(b().x)),
			z(static_cast<T>(b().y)) { }
#endif

		constexpr inline T& operator[](usize i) {
			ASSERT(i >= 0 && i < 3, "vec3 subscript out of range");
			if (i == 0) return x;
			else if (i == 1) return y;
			return z;
		}

		constexpr inline const T& operator[](usize i) const {
			ASSERT(i >= 0 && i < 3, "vec3 subscript out of range");
			if (i == 0) return x;
			else if (i == 1) return y;
			return z;
		}

		constexpr inline bool operator==(const vec<3, T>& other) const {
			return x == other.x && y == other.y && z == other.z;
		}

		constexpr inline bool operator!=(const vec<3, T>& other) const {
			return x != other.x || y != other.y || z != other.z;
		}

		constexpr inline void operator+=(const vec<3, T>& other) {
			x += other.x;
			y += other.y;
			z += other.z;
		}

		constexpr inline void operator-=(const vec<3, T>& other) {
			x -= other.x;
			y -= other.y;
			z -= other.z;
		}

		constexpr inline void operator*=(const vec<3, T>& other) {
			x *= other.x;
			y *= other.y;
			z *= other.z;
		}

		constexpr inline void operator/=(const vec<3, T>& other) {
			x /= other.x;
			y /= other.y;
			z /= other.z;
		}

#ifdef MATH_ENABLE_SWIZZLE
		template<i32 E0, i32 E1, i32 E2>
		constexpr inline bool operator==(const detail::Swizzle<T, 3, E0, E1, E2, -1>& other) const {
			return x == other().x && y == other().y && z == other().z;
		}

		template<i32 E0, i32 E1, i32 E2>
		constexpr inline bool operator!=(const detail::Swizzle<T, 3, E0, E1, E2, -1>& other) const {
			return x != other().x || y != other().y || z != other().z;
		}

		template<i32 E0, i32 E1, i32 E2>
		constexpr inline void operator+=(const detail::Swizzle<T, 3, E0, E1, E2, -1>& other) {
			x += other().x;
			y += other().y;
			z += other().z;
		}

		template<i32 E0, i32 E1, i32 E2>
		constexpr inline void operator-=(const detail::Swizzle<T, 3, E0, E1, E2, -1>& other) {
			x -= other().x;
			y -= other().y;
			z -= other().z;
		}

		template<i32 E0, i32 E1, i32 E2>
		constexpr inline void operator*=(const detail::Swizzle<T, 3, E0, E1, E2, -1>& other) {
			x *= other().x;
			y *= other().y;
			z *= other().z;
		}

		template<i32 E0, i32 E1, i32 E2>
		constexpr inline void operator/=(const detail::Swizzle<T, 3, E0, E1, E2, -1>& other) {
			x /= other().x;
			y /= other().y;
			z /= other().z;
		}
#endif

		constexpr inline void operator*=(T s) {
			x *= s;
			y *= s;
			z *= s;
		}

		constexpr inline void operator/=(T s) {
			const typename types::IntToFloat<T>::Type invS = 1.0 / s;
			x *= invS;
			y *= invS;
			z *= invS;
		}

		constexpr inline vec<3, T> operator-() const {
			return vec<3, T>(-x, -y, -z);
		}

		constexpr inline vec<3, T> operator+() const { return *this; }

		union {
			struct { T x, y, z; };
			struct { T r, g, b; };
			struct { T u, v, w; };
			struct { T s, t, p; };

#ifdef MATH_ENABLE_SWIZZLE
			detail::Swizzle<T, 2, 0, 0, -1, -1> xx, rr, uu, ss;
			detail::Swizzle<T, 2, 1, 0, -1, -1> yx, gr, vu, ts;
			detail::Swizzle<T, 2, 2, 0, -1, -1> zx, br, wu, ps;
			detail::Swizzle<T, 2, 0, 1, -1, -1> xy, rg, uv, st;
			detail::Swizzle<T, 2, 1, 1, -1, -1> yy, gg, vv, tt;
			detail::Swizzle<T, 2, 2, 1, -1, -1> zy, bg, wv, pt;
			detail::Swizzle<T, 2, 0, 2, -1, -1> xz, rb, uw, sp;
			detail::Swizzle<T, 2, 1, 2, -1, -1> yz, gb, vw, tp;
			detail::Swizzle<T, 2, 2, 2, -1, -1> zz, bb, ww, pp;

			detail::Swizzle<T, 3, 0, 0, 0, -1> xxx, rrr, uuu, sss;
			detail::Swizzle<T, 3, 1, 0, 0, -1> yxx, grr, vuu, tss;
			detail::Swizzle<T, 3, 2, 0, 0, -1> zxx, brr, wuu, pss;
			detail::Swizzle<T, 3, 0, 1, 0, -1> xyx, rgr, uvu, sts;
			detail::Swizzle<T, 3, 1, 1, 0, -1> yyx, ggr, vvu, tts;
			detail::Swizzle<T, 3, 2, 1, 0, -1> zyx, bgr, wvu, pts;
			detail::Swizzle<T, 3, 0, 2, 0, -1> xzx, rbr, uwu, sps;
			detail::Swizzle<T, 3, 1, 2, 0, -1> yzx, gbr, vwu, tps;
			detail::Swizzle<T, 3, 2, 2, 0, -1> zzx, bbr, wwu, pps;
			detail::Swizzle<T, 3, 0, 0, 1, -1> xxy, rrg, uuv, sst;
			detail::Swizzle<T, 3, 1, 0, 1, -1> yxy, grg, vuv, tst;
			detail::Swizzle<T, 3, 2, 0, 1, -1> zxy, brg, wuv, pst;
			detail::Swizzle<T, 3, 0, 1, 1, -1> xyy, rgg, uvv, stt;
			detail::Swizzle<T, 3, 1, 1, 1, -1> yyy, ggg, vvv, ttt;
			detail::Swizzle<T, 3, 2, 1, 1, -1> zyy, bgg, wvv, ptt;
			detail::Swizzle<T, 3, 0, 2, 1, -1> xzy, rbg, uwv, spt;
			detail::Swizzle<T, 3, 1, 2, 1, -1> yzy, gbg, vwv, tpt;
			detail::Swizzle<T, 3, 2, 2, 1, -1> zzy, bbg, wwv, ppt;
			detail::Swizzle<T, 3, 0, 0, 2, -1> xxz, rrb, uuw, ssp;
			detail::Swizzle<T, 3, 1, 0, 2, -1> yxz, grb, vuw, tsp;
			detail::Swizzle<T, 3, 2, 0, 2, -1> zxz, brb, wuw, psp;
			detail::Swizzle<T, 3, 0, 1, 2, -1> xyz, rgb, uvw, stp;
			detail::Swizzle<T, 3, 1, 1, 2, -1> yyz, ggb, vvw, ttp;
			detail::Swizzle<T, 3, 2, 1, 2, -1> zyz, bgb, wvw, ptp;
			detail::Swizzle<T, 3, 0, 2, 2, -1> xzz, rbb, uww, spp;
			detail::Swizzle<T, 3, 1, 2, 2, -1> yzz, gbb, vww, tpp;
			detail::Swizzle<T, 3, 2, 2, 2, -1> zzz, bbb, www, ppp;

			detail::Swizzle<T, 4, 0, 0, 0, 0> xxxx, rrrr, uuuu, ssss;
			detail::Swizzle<T, 4, 1, 0, 0, 0> yxxx, grrr, vuuu, tsss;
			detail::Swizzle<T, 4, 2, 0, 0, 0> zxxx, brrr, wuuu, psss;
			detail::Swizzle<T, 4, 0, 1, 0, 0> xyxx, rgrr, uvuu, stss;
			detail::Swizzle<T, 4, 1, 1, 0, 0> yyxx, ggrr, vvuu, ttss;
			detail::Swizzle<T, 4, 2, 1, 0, 0> zyxx, bgrr, wvuu, ptss;
			detail::Swizzle<T, 4, 0, 2, 0, 0> xzxx, rbrr, uwuu, spss;
			detail::Swizzle<T, 4, 1, 2, 0, 0> yzxx, gbrr, vwuu, tpss;
			detail::Swizzle<T, 4, 2, 2, 0, 0> zzxx, bbrr, wwuu, ppss;
			detail::Swizzle<T, 4, 0, 0, 1, 0> xxyx, rrgr, uuvu, ssts;
			detail::Swizzle<T, 4, 1, 0, 1, 0> yxyx, grgr, vuvu, tsts;
			detail::Swizzle<T, 4, 2, 0, 1, 0> zxyx, brgr, wuvu, psts;
			detail::Swizzle<T, 4, 0, 1, 1, 0> xyyx, rggr, uvvu, stts;
			detail::Swizzle<T, 4, 1, 1, 1, 0> yyyx, gggr, vvvu, ttts;
			detail::Swizzle<T, 4, 2, 1, 1, 0> zyyx, bggr, wvvu, ptts;
			detail::Swizzle<T, 4, 0, 2, 1, 0> xzyx, rbgr, uwvu, spts;
			detail::Swizzle<T, 4, 1, 2, 1, 0> yzyx, gbgr, vwvu, tpts;
			detail::Swizzle<T, 4, 2, 2, 1, 0> zzyx, bbgr, wwvu, ppts;
			detail::Swizzle<T, 4, 0, 0, 2, 0> xxzx, rrbr, uuwu, ssps;
			detail::Swizzle<T, 4, 1, 0, 2, 0> yxzx, grbr, vuwu, tsps;
			detail::Swizzle<T, 4, 2, 0, 2, 0> zxzx, brbr, wuwu, psps;
			detail::Swizzle<T, 4, 0, 1, 2, 0> xyzx, rgbr, uvwu, stps;
			detail::Swizzle<T, 4, 1, 1, 2, 0> yyzx, ggbr, vvwu, ttps;
			detail::Swizzle<T, 4, 2, 1, 2, 0> zyzx, bgbr, wvwu, ptps;
			detail::Swizzle<T, 4, 0, 2, 2, 0> xzzx, rbbr, uwwu, spps;
			detail::Swizzle<T, 4, 1, 2, 2, 0> yzzx, gbbr, vwwu, tpps;
			detail::Swizzle<T, 4, 2, 2, 2, 0> zzzx, bbbr, wwwu, ppps;
			detail::Swizzle<T, 4, 0, 0, 0, 1> xxxy, rrrg, uuuv, ssst;
			detail::Swizzle<T, 4, 1, 0, 0, 1> yxxy, grrg, vuuv, tsst;
			detail::Swizzle<T, 4, 2, 0, 0, 1> zxxy, brrg, wuuv, psst;
			detail::Swizzle<T, 4, 0, 1, 0, 1> xyxy, rgrg, uvuv, stst;
			detail::Swizzle<T, 4, 1, 1, 0, 1> yyxy, ggrg, vvuv, ttst;
			detail::Swizzle<T, 4, 2, 1, 0, 1> zyxy, bgrg, wvuv, ptst;
			detail::Swizzle<T, 4, 0, 2, 0, 1> xzxy, rbrg, uwuv, spst;
			detail::Swizzle<T, 4, 1, 2, 0, 1> yzxy, gbrg, vwuv, tpst;
			detail::Swizzle<T, 4, 2, 2, 0, 1> zzxy, bbrg, wwuv, ppst;
			detail::Swizzle<T, 4, 0, 0, 1, 1> xxyy, rrgg, uuvv, sstt;
			detail::Swizzle<T, 4, 1, 0, 1, 1> yxyy, grgg, vuvv, tstt;
			detail::Swizzle<T, 4, 2, 0, 1, 1> zxyy, brgg, wuvv, pstt;
			detail::Swizzle<T, 4, 0, 1, 1, 1> xyyy, rggg, uvvv, sttt;
			detail::Swizzle<T, 4, 1, 1, 1, 1> yyyy, gggg, vvvv, tttt;
			detail::Swizzle<T, 4, 2, 1, 1, 1> zyyy, bggg, wvvv, pttt;
			detail::Swizzle<T, 4, 0, 2, 1, 1> xzyy, rbgg, uwvv, sptt;
			detail::Swizzle<T, 4, 1, 2, 1, 1> yzyy, gbgg, vwvv, tptt;
			detail::Swizzle<T, 4, 2, 2, 1, 1> zzyy, bbgg, wwvv, pptt;
			detail::Swizzle<T, 4, 0, 0, 2, 1> xxzy, rrbg, uuwv, sspt;
			detail::Swizzle<T, 4, 1, 0, 2, 1> yxzy, grbg, vuwv, tspt;
			detail::Swizzle<T, 4, 2, 0, 2, 1> zxzy, brbg, wuwv, pspt;
			detail::Swizzle<T, 4, 0, 1, 2, 1> xyzy, rgbg, uvwv, stpt;
			detail::Swizzle<T, 4, 1, 1, 2, 1> yyzy, ggbg, vvwv, ttpt;
			detail::Swizzle<T, 4, 2, 1, 2, 1> zyzy, bgbg, wvwv, ptpt;
			detail::Swizzle<T, 4, 0, 2, 2, 1> xzzy, rbbg, uwwv, sppt;
			detail::Swizzle<T, 4, 1, 2, 2, 1> yzzy, gbbg, vwwv, tppt;
			detail::Swizzle<T, 4, 2, 2, 2, 1> zzzy, bbbg, wwwv, pppt;
			detail::Swizzle<T, 4, 0, 0, 0, 2> xxxz, rrrb, uuuw, sssp;
			detail::Swizzle<T, 4, 1, 0, 0, 2> yxxz, grrb, vuuw, tssp;
			detail::Swizzle<T, 4, 2, 0, 0, 2> zxxz, brrb, wuuw, pssp;
			detail::Swizzle<T, 4, 0, 1, 0, 2> xyxz, rgrb, uvuw, stsp;
			detail::Swizzle<T, 4, 1, 1, 0, 2> yyxz, ggrb, vvuw, ttsp;
			detail::Swizzle<T, 4, 2, 1, 0, 2> zyxz, bgrb, wvuw, ptsp;
			detail::Swizzle<T, 4, 0, 2, 0, 2> xzxz, rbrb, uwuw, spsp;
			detail::Swizzle<T, 4, 1, 2, 0, 2> yzxz, gbrb, vwuw, tpsp;
			detail::Swizzle<T, 4, 2, 2, 0, 2> zzxz, bbrb, wwuw, ppsp;
			detail::Swizzle<T, 4, 0, 0, 1, 2> xxyz, rrgb, uuvw, sstp;
			detail::Swizzle<T, 4, 1, 0, 1, 2> yxyz, grgb, vuvw, tstp;
			detail::Swizzle<T, 4, 2, 0, 1, 2> zxyz, brgb, wuvw, pstp;
			detail::Swizzle<T, 4, 0, 1, 1, 2> xyyz, rggb, uvvw, sttp;
			detail::Swizzle<T, 4, 1, 1, 1, 2> yyyz, gggb, vvvw, tttp;
			detail::Swizzle<T, 4, 2, 1, 1, 2> zyyz, bggb, wvvw, pttp;
			detail::Swizzle<T, 4, 0, 2, 1, 2> xzyz, rbgb, uwvw, sptp;
			detail::Swizzle<T, 4, 1, 2, 1, 2> yzyz, gbgb, vwvw, tptp;
			detail::Swizzle<T, 4, 2, 2, 1, 2> zzyz, bbgb, wwvw, pptp;
			detail::Swizzle<T, 4, 0, 0, 2, 2> xxzz, rrbb, uuww, sspp;
			detail::Swizzle<T, 4, 1, 0, 2, 2> yxzz, grbb, vuww, tspp;
			detail::Swizzle<T, 4, 2, 0, 2, 2> zxzz, brbb, wuww, pspp;
			detail::Swizzle<T, 4, 0, 1, 2, 2> xyzz, rgbb, uvww, stpp;
			detail::Swizzle<T, 4, 1, 1, 2, 2> yyzz, ggbb, vvww, ttpp;
			detail::Swizzle<T, 4, 2, 1, 2, 2> zyzz, bgbb, wvww, ptpp;
			detail::Swizzle<T, 4, 0, 2, 2, 2> xzzz, rbbb, uwww, sppp;
			detail::Swizzle<T, 4, 1, 2, 2, 2> yzzz, gbbb, vwww, tppp;
			detail::Swizzle<T, 4, 2, 2, 2, 2> zzzz, bbbb, wwww, pppp;
#endif
		};
	};

	template<Numeric T>
	const vec<3, T> vec<3, T>::Up = vec<3, T>(0, 1, 0);

	template<Numeric T>
	const vec<3, T> vec<3, T>::Down = vec<3, T>(0, -1, 0);

	template<Numeric T>
	const vec<3, T> vec<3, T>::Right = vec<3, T>(1, 0, 0);

	template<Numeric T>
	const vec<3, T> vec<3, T>::Left = vec<3, T>(-1, 0, 0);

	template<Numeric T>
	const vec<3, T> vec<3, T>::Forward = vec<3, T>(0, 0, 1);

	template<Numeric T>
	const vec<3, T> vec<3, T>::Backward = vec<3, T>(0, 0, -1);

	template<Numeric T>
	constexpr inline T Dot(const vec<3, T>& a, const vec<3, T>& b) {
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	template<Numeric T>
	constexpr inline vec<3, T> Cross(const vec<3, T>& a, const vec<3, T>& b) {
		return vec<3, T>(
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x);
	}

	template<Numeric T>
	constexpr inline vec<3, T> Abs(const vec<3, T>& v) {
		return vec<3, T>(Abs(v.x), Abs(v.y), Abs(v.z));
	}

	template<Numeric T>
	constexpr inline vec<3, T> Min(const vec<3, T>& a, const vec<3, T>& b) {
		return vec<3, T>(Min(a.x, b.x), Min(a.y, b.y), Min(a.z, b.z));
	}

	template<Numeric T>
	constexpr inline vec<3, T> Max(const vec<3, T>& a, const vec<3, T>& b) {
		return vec<3, T>(Max(a.x, b.x), Max(a.y, b.y), Max(a.z, b.z));
	}

	template<Floating T>
	constexpr inline vec<3, T> Floor(const vec<3, T>& v) {
		return vec<3, T>(Floor(v.x), Floor(v.y), Floor(v.z));
	}

	template<Floating T>
	constexpr inline vec<3, T> Ceil(const vec<3, T>& v) {
		return vec<3, T>(Ceil(v.x), Ceil(v.y), Ceil(v.z));
	}

	typedef vec<3, f32>  vec3;
	typedef vec<3, f64> dvec3;
	typedef vec<3, i32> ivec3;
	typedef vec<3, u32> uvec3;
	typedef vec<3, b8>  bvec3;
}

template<math::Numeric T>
constexpr inline math::vec<3, T> operator+(const math::vec<3, T>& a, const math::vec<3, T>& b) {
	return math::vec<3, T>(a.x + b.x, a.y + b.y, a.z + b.z);
}

template<math::Numeric T>
constexpr inline math::vec<3, T> operator-(const math::vec<3, T>& a, const math::vec<3, T>& b) {
	return math::vec<3, T>(a.x - b.x, a.y - b.y, a.z - b.z);
}

template<math::Numeric T>
constexpr inline math::vec<3, T> operator*(const math::vec<3, T>& a, const math::vec<3, T>& b) {
	return math::vec<3, T>(a.x * b.x, a.y * b.y, a.z * b.z);
}

template<math::Numeric T>
constexpr inline math::vec<3, T> operator/(const math::vec<3, T>& a, const math::vec<3, T>& b) {
	return math::vec<3, T>(a.x / b.x, a.y / b.y, a.z / b.z);
}

template<math::Numeric T>
constexpr inline math::vec<3, T> operator%(const math::vec<3, T>& a, const math::vec<3, T>& b) {
	return math::vec<3, T>(a.x % b.x, a.y % b.y, a.z % b.z);
}

template<math::Numeric T>
constexpr inline math::vec<3, T> operator*(const math::vec<3, T>& v, T s) {
	return math::vec<3, T>(v.x * s, v.y * s, v.z * s);
}

template<math::Numeric T>
constexpr inline math::vec<3, T> operator/(const math::vec<3, T>& v, T s) {
	const typename types::IntToFloat<T>::Type invS = 1.0 / s;
	return math::vec<3, T>(v.x * invS, v.y * invS, v.z * invS);
}