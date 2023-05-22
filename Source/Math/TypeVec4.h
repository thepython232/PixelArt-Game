#pragma once

#include "Common.h"
#include "TypeVec.h"
#ifdef MATH_ENABLE_SWIZZLE
#include "Swizzle.h"
#endif

namespace math {

	template<Numeric T>
	struct vec<4, T> {
		using Type = vec<4, T>;
		using DataType = T;
		static constexpr usize Length = 4;

		constexpr vec() = default;
		constexpr explicit vec(T s)
			: x(s), y(s), z(s), w(s) { }
		constexpr vec(T x, T y, T z, T w)
			: x(x), y(y), z(z), w(w) { }

		constexpr vec(const vec<4, T>& other)
			: x(other.x), y(other.y), z(other.z), w(other.w) { }

		template<Numeric U>
		constexpr explicit vec(const vec<4, U>& other)
			: x(static_cast<T>(other.x)),
			y(static_cast<T>(other.y)),
			z(static_cast<T>(other.z)),
			w(static_cast<T>(other.w)) { }

		constexpr vec(const vec<3, T>& a, T b)
			: x(a.x), y(a.y), z(a.z), w(b) { }

		constexpr vec(T a, const vec<3, T>& b)
			: x(a), y(b.x), z(b.y), w(b.z) { }

		constexpr vec(const vec<2, T>&a, T b, T c)
			: x(a.x), y(a.y), z(b), w(c) { }

		constexpr vec(T a, const vec<2, T>& b, T c)
			: x(a), y(b.x), z(b.y), w(c) { }

		constexpr vec(T a, T b, const vec<2, T>& c)
			: x(a), y(b), z(c.x), w(c.y) { }

		constexpr vec(const vec<2, T>& a, const vec<2, T>& b)
			: x(a.x), y(a.y), z(b.x), w(b.y) { }

		template<Numeric U>
		constexpr explicit vec(const vec<3, U>& a, U b)
			: x(static_cast<T>(a.x)),
			y(static_cast<T>(a.y)),
			z(static_cast<T>(a.z)),
			w(static_cast<T>(b)) { }

		template<Numeric U>
		constexpr explicit vec(T a, const vec<3, U>& b)
			: x(static_cast<T>(a)),
			y(static_cast<T>(b.x)),
			z(static_cast<T>(b.y)),
			w(static_cast<T>(b.z)) { }

		template<Numeric U>
		constexpr explicit vec(const vec<2, U>& a, U b, U c)
			: x(static_cast<T>(a.x)),
			y(static_cast<T>(a.y)),
			z(static_cast<T>(b)),
			w(static_cast<T>(c)) { }

		template<Numeric U>
		constexpr explicit vec(T a, const vec<2, U>& b, U c)
			: x(static_cast<T>(a)),
			y(static_cast<T>(b.x)),
			z(static_cast<T>(b.y)),
			w(static_cast<T>(c)) { }

		template<Numeric U>
		constexpr explicit vec(T a, U b, const vec<2, U>& c)
			: x(static_cast<T>(a)),
			y(static_cast<T>(b)),
			z(static_cast<T>(c.x)),
			w(static_cast<T>(c.y)) { }

		template<Numeric U, Numeric V>
		constexpr explicit vec(const vec<2, U>& a, const vec<2, V>& b)
			: x(static_cast<T>(a.x)),
			y(static_cast<T>(a.y)),
			z(static_cast<T>(b.x)),
			w(static_cast<T>(b.y)) { }

#ifdef MATH_ENABLE_SWIZZLE
		template<i32 E0, i32 E1, i32 E2, i32 E3>
		constexpr vec(const detail::Swizzle<T, 4, E0, E1, E2, E3>& other)
			: x(other().x), y(other().y), z(other().z), w(other().w) { }

		template<Numeric U, i32 E0, i32 E1, i32 E2, i32 E3>
		constexpr explicit vec(const detail::Swizzle<U, 4, E0, E1, E2, E3>& other)
			: x(static_cast<T>(other().x)),
			y(static_cast<T>(other().y)),
			z(static_cast<T>(other().z)),
			w(static_cast<T>(other().w)) { }

		template<i32 E0, i32 E1, i32 E2>
		constexpr vec(const detail::Swizzle<T, 3, E0, E1, E2, -1>& a, T b)
			: x(a().x), y(a().y), z(a().z), w(b) { }

		template<i32 E0, i32 E1, i32 E2>
		constexpr vec(T a, const detail::Swizzle<T, 3, E0, E1, E2, -1>& b)
			: x(a), y(b().x), z(b().y), w(b().z) { }

		template<i32 E0, i32 E1>
		constexpr vec(const detail::Swizzle<T, 2, E0, E1, -1, -1>& a, T b, T c)
			: x(a().x), y(a().y), z(b), w(c) { }

		template<i32 E0, i32 E1>
		constexpr vec(T a, const detail::Swizzle<T, 2, E0, E1, -1, -1>& b, T c)
			: x(a), y(b().x), z(b().y), w(c) { }

		template<i32 E0, i32 E1>
		constexpr vec(T a, T b, const detail::Swizzle<T, 2, E0, E1, -1, -1>& c)
			: x(a), y(b), z(c().x), w(c.y) { }

		template<i32 AE0, i32 AE1, i32 BE0, i32 BE1>
		constexpr vec(const vec<2, T>& a, const vec<2, T>& b)
			: x(a().x), y(a().y), z(b().x), w(b().y) { }

		template<Numeric U, i32 E0, i32 E1, i32 E2>
		constexpr explicit vec(const detail::Swizzle<T, 3, E0, E1, E2, -1>& a, U b)
			: x(static_cast<T>(a().x)),
			y(static_cast<T>(a().y)),
			z(static_cast<T>(a().z)),
			w(static_cast<T>(b)) { }

		template<Numeric U, i32 E0, i32 E1, i32 E2>
		constexpr explicit vec(T a, const detail::Swizzle<T, 3, E0, E1, E2, -1>& b)
			: x(static_cast<T>(a)),
			y(static_cast<T>(b().x)),
			z(static_cast<T>(b().y)),
			w(static_cast<T>(b().z)) { }

		template<Numeric U, i32 E0, i32 E1>
		constexpr explicit vec(const detail::Swizzle<T, 2, E0, E1, -1, -1>& a, U b, U c)
			: x(static_cast<T>(a().x)),
			y(static_cast<T>(a().y)),
			z(static_cast<T>(b)),
			w(static_cast<T>(c)) { }

		template<Numeric U, i32 E0, i32 E1>
		constexpr explicit vec(T a, const detail::Swizzle<T, 2, E0, E1, -1, -1>& b, U c)
			: x(static_cast<T>(a)),
			y(static_cast<T>(b().x)),
			z(static_cast<T>(b().y)),
			w(static_cast<T>(c)) { }

		template<Numeric U, i32 E0, i32 E1>
		constexpr explicit vec(T a, U b, const detail::Swizzle<T, 2, E0, E1, -1, -1>& c)
			: x(static_cast<T>(a)),
			y(static_cast<T>(b)),
			z(static_cast<T>(c().x)),
			w(static_cast<T>(c().y)) { }

		template<Numeric U, Numeric V,
			i32 AE0, i32 AE1,
			i32 BE0, i32 BE1>
		constexpr explicit vec(const detail::Swizzle<T, 2, AE0, AE1, -1, -1>& a,
			const detail::Swizzle<T, 2, BE0, BE1, -1, -1>& b)
			: x(static_cast<T>(a().x)),
			y(static_cast<T>(a().y)),
			z(static_cast<T>(b().x)),
			w(static_cast<T>(b().y)) { }
#endif

		constexpr inline T& operator[](usize i) {
			ASSERT(i >= 0 && i < 4, "vec4 subscript out of range!");
			if (i == 0) return x;
			else if (i == 1) return y;
			else if (i == 2) return z;
			return w;
		}

		constexpr inline const T& operator[](usize i) const {
			ASSERT(i >= 0 && i < 4, "vec4 subscript out of range!");
			if (i == 0) return x;
			else if (i == 1) return y;
			else if (i == 2) return z;
			return w;
		}

		constexpr inline bool operator==(const vec<4, T>& other) const {
			return x == other.x && y == other.y && z == other.z && w == other.w;
		}

		constexpr inline bool operator!=(const vec<4, T>& other) const {
			return x != other.x || y != other.y || z != other.z || w != other.w;
		}

		inline void operator+=(const vec<4, T>& other) {
			x += other.x;
			y += other.y;
			z += other.z;
			w += other.w;
		}

		inline void operator-=(const vec<4, T>& other) {
			x -= other.x;
			y -= other.y;
			z -= other.z;
			w -= other.w;
		}

		inline void operator*=(const vec<4, T>& other) {
			x *= other.x;
			y *= other.y;
			z *= other.z;
			w *= other.w;
		}

		inline void operator/=(const vec<4, T>& other) {
			x /= other.x;
			y /= other.y;
			z /= other.z;
			w /= other.w;
		}

		inline void operator*=(T s) {
			x *= s;
			y *= s;
			z *= s;
			w *= s;
		}

		inline void operator/=(T s) {
			const typename types::IntToFloat<T>::Type invS = 1.0 / s;
			x *= invS;
			y *= invS;
			z *= invS;
			w *= invS;
		}

#ifdef MATH_ENABLE_SWIZZLE
		template<i32 E0, i32 E1, i32 E2, i32 E3>
		constexpr inline bool operator==(const detail::Swizzle<T, 4, E0, E1, E2, E3>& other) const {
			return x == other().x && y == other().y && z == other().z && w == other().w;
		}

		template<i32 E0, i32 E1, i32 E2, i32 E3>
		constexpr inline bool operator!=(const detail::Swizzle<T, 4, E0, E1, E2, E3>& other) const {
			return x != other().x || y != other().y || z != other().z || w != other().w;
		}

		template<i32 E0, i32 E1, i32 E2, i32 E3>
		inline void operator+=(const detail::Swizzle<T, 4, E0, E1, E2, E3>& other) {
			x += other().x;
			y += other().y;
			z += other().z;
			w += other().w;
		}

		template<i32 E0, i32 E1, i32 E2, i32 E3>
		inline void operator-=(const detail::Swizzle<T, 4, E0, E1, E2, E3>& other) {
			x -= other().x;
			y -= other().y;
			z -= other().z;
			w -= other().w;
		}

		template<i32 E0, i32 E1, i32 E2, i32 E3>
		inline void operator*=(const detail::Swizzle<T, 4, E0, E1, E2, E3>& other) {
			x *= other().x;
			y *= other().y;
			z *= other().z;
			w *= other().w;
		}

		template<i32 E0, i32 E1, i32 E2, i32 E3>
		inline void operator/=(const detail::Swizzle<T, 4, E0, E1, E2, E3>& other) {
			x /= other().x;
			y /= other().y;
			z /= other().z;
			w /= other().w;
		}
#endif

		constexpr inline vec<4, T> operator-() const {
			return vec<4, T>(-x, -y, -z, -w);
		}

		constexpr inline vec<4, T> operator+() const { return *this; }

		union {
			struct { T x, y, z, w; };
			struct { T r, g, b, a; };
			struct { T s, t, p, q; };

#ifdef MATH_ENABLE_SWIZZLE
			detail::Swizzle<T, 2, 0, 0, -1, -1> xx, rr, ss;
			detail::Swizzle<T, 2, 1, 0, -1, -1> yx, gr, ts;
			detail::Swizzle<T, 2, 2, 0, -1, -1> zx, br, ps;
			detail::Swizzle<T, 2, 3, 0, -1, -1> wx, ar, qs;
			detail::Swizzle<T, 2, 0, 1, -1, -1> xy, rg, st;
			detail::Swizzle<T, 2, 1, 1, -1, -1> yy, gg, tt;
			detail::Swizzle<T, 2, 2, 1, -1, -1> zy, bg, pt;
			detail::Swizzle<T, 2, 3, 1, -1, -1> wy, ag, qt;
			detail::Swizzle<T, 2, 0, 2, -1, -1> xz, rb, sp;
			detail::Swizzle<T, 2, 1, 2, -1, -1> yz, gb, tp;
			detail::Swizzle<T, 2, 2, 2, -1, -1> zz, bb, pp;
			detail::Swizzle<T, 2, 3, 2, -1, -1> wz, ab, qp;
			detail::Swizzle<T, 2, 0, 3, -1, -1> xw, ra, sq;
			detail::Swizzle<T, 2, 1, 3, -1, -1> yw, ga, tq;
			detail::Swizzle<T, 2, 2, 3, -1, -1> zw, ba, pq;
			detail::Swizzle<T, 2, 3, 3, -1, -1> ww, aa, qq;

			detail::Swizzle<T, 3, 0, 0, 0, -1> xxx, rrr, sss;
			detail::Swizzle<T, 3, 1, 0, 0, -1> yxx, grr, tss;
			detail::Swizzle<T, 3, 2, 0, 0, -1> zxx, brr, pss;
			detail::Swizzle<T, 3, 3, 0, 0, -1> wxx, arr, qss;
			detail::Swizzle<T, 3, 0, 1, 0, -1> xyx, rgr, sts;
			detail::Swizzle<T, 3, 1, 1, 0, -1> yyx, ggr, tts;
			detail::Swizzle<T, 3, 2, 1, 0, -1> zyx, bgr, pts;
			detail::Swizzle<T, 3, 3, 1, 0, -1> wyx, agr, qts;
			detail::Swizzle<T, 3, 0, 2, 0, -1> xzx, rbr, sps;
			detail::Swizzle<T, 3, 1, 2, 0, -1> yzx, gbr, tps;
			detail::Swizzle<T, 3, 2, 2, 0, -1> zzx, bbr, pps;
			detail::Swizzle<T, 3, 3, 2, 0, -1> wzx, abr, qps;
			detail::Swizzle<T, 3, 0, 3, 0, -1> xwx, rar, sqs;
			detail::Swizzle<T, 3, 1, 3, 0, -1> ywx, gar, tqs;
			detail::Swizzle<T, 3, 2, 3, 0, -1> zwx, bar, pqs;
			detail::Swizzle<T, 3, 3, 3, 0, -1> wwx, aar, qqs;
			detail::Swizzle<T, 3, 0, 0, 1, -1> xxy, rrg, sst;
			detail::Swizzle<T, 3, 1, 0, 1, -1> yxy, grg, tst;
			detail::Swizzle<T, 3, 2, 0, 1, -1> zxy, brg, pst;
			detail::Swizzle<T, 3, 3, 0, 1, -1> wxy, arg, qst;
			detail::Swizzle<T, 3, 0, 1, 1, -1> xyy, rgg, stt;
			detail::Swizzle<T, 3, 1, 1, 1, -1> yyy, ggg, ttt;
			detail::Swizzle<T, 3, 2, 1, 1, -1> zyy, bgg, ptt;
			detail::Swizzle<T, 3, 3, 1, 1, -1> wyy, agg, qtt;
			detail::Swizzle<T, 3, 0, 2, 1, -1> xzy, rbg, spt;
			detail::Swizzle<T, 3, 1, 2, 1, -1> yzy, gbg, tpt;
			detail::Swizzle<T, 3, 2, 2, 1, -1> zzy, bbg, ppt;
			detail::Swizzle<T, 3, 3, 2, 1, -1> wzy, abg, qpt;
			detail::Swizzle<T, 3, 0, 3, 1, -1> xwy, rag, sqt;
			detail::Swizzle<T, 3, 1, 3, 1, -1> ywy, gag, tqt;
			detail::Swizzle<T, 3, 2, 3, 1, -1> zwy, bag, pqt;
			detail::Swizzle<T, 3, 3, 3, 1, -1> wwy, aag, qqt;
			detail::Swizzle<T, 3, 0, 0, 2, -1> xxz, rrb, ssp;
			detail::Swizzle<T, 3, 1, 0, 2, -1> yxz, grb, tsp;
			detail::Swizzle<T, 3, 2, 0, 2, -1> zxz, brb, psp;
			detail::Swizzle<T, 3, 3, 0, 2, -1> wxz, arb, qsp;
			detail::Swizzle<T, 3, 0, 1, 2, -1> xyz, rgb, stp;
			detail::Swizzle<T, 3, 1, 1, 2, -1> yyz, ggb, ttp;
			detail::Swizzle<T, 3, 2, 1, 2, -1> zyz, bgb, ptp;
			detail::Swizzle<T, 3, 3, 1, 2, -1> wyz, agb, qtp;
			detail::Swizzle<T, 3, 0, 2, 2, -1> xzz, rbb, spp;
			detail::Swizzle<T, 3, 1, 2, 2, -1> yzz, gbb, tpp;
			detail::Swizzle<T, 3, 2, 2, 2, -1> zzz, bbb, ppp;
			detail::Swizzle<T, 3, 3, 2, 2, -1> wzz, abb, qpp;
			detail::Swizzle<T, 3, 0, 3, 2, -1> xwz, rab, sqp;
			detail::Swizzle<T, 3, 1, 3, 2, -1> ywz, gab, tqp;
			detail::Swizzle<T, 3, 2, 3, 2, -1> zwz, bab, pqp;
			detail::Swizzle<T, 3, 3, 3, 2, -1> wwz, aab, qqp;
			detail::Swizzle<T, 3, 0, 0, 3, -1> xxw, rra, ssq;
			detail::Swizzle<T, 3, 1, 0, 3, -1> yxw, gra, tsq;
			detail::Swizzle<T, 3, 2, 0, 3, -1> zxw, bra, psq;
			detail::Swizzle<T, 3, 3, 0, 3, -1> wxw, ara, qsq;
			detail::Swizzle<T, 3, 0, 1, 3, -1> xyw, rga, stq;
			detail::Swizzle<T, 3, 1, 1, 3, -1> yyw, gga, ttq;
			detail::Swizzle<T, 3, 2, 1, 3, -1> zyw, bga, ptq;
			detail::Swizzle<T, 3, 3, 1, 3, -1> wyw, aga, qtq;
			detail::Swizzle<T, 3, 0, 2, 3, -1> xzw, rba, spq;
			detail::Swizzle<T, 3, 1, 2, 3, -1> yzw, gba, tpq;
			detail::Swizzle<T, 3, 2, 2, 3, -1> zzw, bba, ppq;
			detail::Swizzle<T, 3, 3, 2, 3, -1> wzw, aba, qpq;
			detail::Swizzle<T, 3, 0, 3, 3, -1> xww, raa, sqq;
			detail::Swizzle<T, 3, 1, 3, 3, -1> yww, gaa, tqq;
			detail::Swizzle<T, 3, 2, 3, 3, -1> zww, baa, pqq;
			detail::Swizzle<T, 3, 3, 3, 3, -1> www, aaa, qqq;

			detail::Swizzle<T, 4, 0, 0, 0, 0> xxxx, rrrr, ssss;
			detail::Swizzle<T, 4, 1, 0, 0, 0> yxxx, grrr, tsss;
			detail::Swizzle<T, 4, 2, 0, 0, 0> zxxx, brrr, psss;
			detail::Swizzle<T, 4, 3, 0, 0, 0> wxxx, arrr, qsss;
			detail::Swizzle<T, 4, 0, 1, 0, 0> xyxx, rgrr, stss;
			detail::Swizzle<T, 4, 1, 1, 0, 0> yyxx, ggrr, ttss;
			detail::Swizzle<T, 4, 2, 1, 0, 0> zyxx, bgrr, ptss;
			detail::Swizzle<T, 4, 3, 1, 0, 0> wyxx, agrr, qtss;
			detail::Swizzle<T, 4, 0, 2, 0, 0> xzxx, rbrr, spss;
			detail::Swizzle<T, 4, 1, 2, 0, 0> yzxx, gbrr, tpss;
			detail::Swizzle<T, 4, 2, 2, 0, 0> zzxx, bbrr, ppss;
			detail::Swizzle<T, 4, 3, 2, 0, 0> wzxx, abrr, qpss;
			detail::Swizzle<T, 4, 0, 3, 0, 0> xwxx, rarr, sqss;
			detail::Swizzle<T, 4, 1, 3, 0, 0> ywxx, garr, tqss;
			detail::Swizzle<T, 4, 2, 3, 0, 0> zwxx, barr, pqss;
			detail::Swizzle<T, 4, 3, 3, 0, 0> wwxx, aarr, qqss;
			detail::Swizzle<T, 4, 0, 0, 1, 0> xxyx, rrgr, ssts;
			detail::Swizzle<T, 4, 1, 0, 1, 0> yxyx, grgr, tsts;
			detail::Swizzle<T, 4, 2, 0, 1, 0> zxyx, brgr, psts;
			detail::Swizzle<T, 4, 3, 0, 1, 0> wxyx, argr, qsts;
			detail::Swizzle<T, 4, 0, 1, 1, 0> xyyx, rggr, stts;
			detail::Swizzle<T, 4, 1, 1, 1, 0> yyyx, gggr, ttts;
			detail::Swizzle<T, 4, 2, 1, 1, 0> zyyx, bggr, ptts;
			detail::Swizzle<T, 4, 3, 1, 1, 0> wyyx, aggr, qtts;
			detail::Swizzle<T, 4, 0, 2, 1, 0> xzyx, rbgr, spts;
			detail::Swizzle<T, 4, 1, 2, 1, 0> yzyx, gbgr, tpts;
			detail::Swizzle<T, 4, 2, 2, 1, 0> zzyx, bbgr, ppts;
			detail::Swizzle<T, 4, 3, 2, 1, 0> wzyx, abgr, qpts;
			detail::Swizzle<T, 4, 0, 3, 1, 0> xwyx, ragr, sqts;
			detail::Swizzle<T, 4, 1, 3, 1, 0> ywyx, gagr, tqts;
			detail::Swizzle<T, 4, 2, 3, 1, 0> zwyx, bagr, pqts;
			detail::Swizzle<T, 4, 3, 3, 1, 0> wwyx, aagr, qqts;
			detail::Swizzle<T, 4, 0, 0, 2, 0> xxzx, rrbr, ssps;
			detail::Swizzle<T, 4, 1, 0, 2, 0> yxzx, grbr, tsps;
			detail::Swizzle<T, 4, 2, 0, 2, 0> zxzx, brbr, psps;
			detail::Swizzle<T, 4, 3, 0, 2, 0> wxzx, arbr, qsps;
			detail::Swizzle<T, 4, 0, 1, 2, 0> xyzx, rgbr, stps;
			detail::Swizzle<T, 4, 1, 1, 2, 0> yyzx, ggbr, ttps;
			detail::Swizzle<T, 4, 2, 1, 2, 0> zyzx, bgbr, ptps;
			detail::Swizzle<T, 4, 3, 1, 2, 0> wyzx, agbr, qtps;
			detail::Swizzle<T, 4, 0, 2, 2, 0> xzzx, rbbr, spps;
			detail::Swizzle<T, 4, 1, 2, 2, 0> yzzx, gbbr, tpps;
			detail::Swizzle<T, 4, 2, 2, 2, 0> zzzx, bbbr, ppps;
			detail::Swizzle<T, 4, 3, 2, 2, 0> wzzx, abbr, qpps;
			detail::Swizzle<T, 4, 0, 3, 2, 0> xwzx, rabr, sqps;
			detail::Swizzle<T, 4, 1, 3, 2, 0> ywzx, gabr, tqps;
			detail::Swizzle<T, 4, 2, 3, 2, 0> zwzx, babr, pqps;
			detail::Swizzle<T, 4, 3, 3, 2, 0> wwzx, aabr, qqps;
			detail::Swizzle<T, 4, 0, 0, 3, 0> xxwx, rrar, ssqs;
			detail::Swizzle<T, 4, 1, 0, 3, 0> yxwx, grar, tsqs;
			detail::Swizzle<T, 4, 2, 0, 3, 0> zxwx, brar, psqs;
			detail::Swizzle<T, 4, 3, 0, 3, 0> wxwx, arar, qsqs;
			detail::Swizzle<T, 4, 0, 1, 3, 0> xywx, rgar, stqs;
			detail::Swizzle<T, 4, 1, 1, 3, 0> yywx, ggar, ttqs;
			detail::Swizzle<T, 4, 2, 1, 3, 0> zywx, bgar, ptqs;
			detail::Swizzle<T, 4, 3, 1, 3, 0> wywx, agar, qtqs;
			detail::Swizzle<T, 4, 0, 2, 3, 0> xzwx, rbar, spqs;
			detail::Swizzle<T, 4, 1, 2, 3, 0> yzwx, gbar, tpqs;
			detail::Swizzle<T, 4, 2, 2, 3, 0> zzwx, bbar, ppqs;
			detail::Swizzle<T, 4, 3, 2, 3, 0> wzwx, abar, qpqs;
			detail::Swizzle<T, 4, 0, 3, 3, 0> xwwx, raar, sqqs;
			detail::Swizzle<T, 4, 1, 3, 3, 0> ywwx, gaar, tqqs;
			detail::Swizzle<T, 4, 2, 3, 3, 0> zwwx, baar, pqqs;
			detail::Swizzle<T, 4, 3, 3, 3, 0> wwwx, aaar, qqqs;
			detail::Swizzle<T, 4, 0, 0, 0, 1> xxxy, rrrg, ssst;
			detail::Swizzle<T, 4, 1, 0, 0, 1> yxxy, grrg, tsst;
			detail::Swizzle<T, 4, 2, 0, 0, 1> zxxy, brrg, psst;
			detail::Swizzle<T, 4, 3, 0, 0, 1> wxxy, arrg, qsst;
			detail::Swizzle<T, 4, 0, 1, 0, 1> xyxy, rgrg, stst;
			detail::Swizzle<T, 4, 1, 1, 0, 1> yyxy, ggrg, ttst;
			detail::Swizzle<T, 4, 2, 1, 0, 1> zyxy, bgrg, ptst;
			detail::Swizzle<T, 4, 3, 1, 0, 1> wyxy, agrg, qtst;
			detail::Swizzle<T, 4, 0, 2, 0, 1> xzxy, rbrg, spst;
			detail::Swizzle<T, 4, 1, 2, 0, 1> yzxy, gbrg, tpst;
			detail::Swizzle<T, 4, 2, 2, 0, 1> zzxy, bbrg, ppst;
			detail::Swizzle<T, 4, 3, 2, 0, 1> wzxy, abrg, qpst;
			detail::Swizzle<T, 4, 0, 3, 0, 1> xwxy, rarg, sqst;
			detail::Swizzle<T, 4, 1, 3, 0, 1> ywxy, garg, tqst;
			detail::Swizzle<T, 4, 2, 3, 0, 1> zwxy, barg, pqst;
			detail::Swizzle<T, 4, 3, 3, 0, 1> wwxy, aarg, qqst;
			detail::Swizzle<T, 4, 0, 0, 1, 1> xxyy, rrgg, sstt;
			detail::Swizzle<T, 4, 1, 0, 1, 1> yxyy, grgg, tstt;
			detail::Swizzle<T, 4, 2, 0, 1, 1> zxyy, brgg, pstt;
			detail::Swizzle<T, 4, 3, 0, 1, 1> wxyy, argg, qstt;
			detail::Swizzle<T, 4, 0, 1, 1, 1> xyyy, rggg, sttt;
			detail::Swizzle<T, 4, 1, 1, 1, 1> yyyy, gggg, tttt;
			detail::Swizzle<T, 4, 2, 1, 1, 1> zyyy, bggg, pttt;
			detail::Swizzle<T, 4, 3, 1, 1, 1> wyyy, aggg, qttt;
			detail::Swizzle<T, 4, 0, 2, 1, 1> xzyy, rbgg, sptt;
			detail::Swizzle<T, 4, 1, 2, 1, 1> yzyy, gbgg, tptt;
			detail::Swizzle<T, 4, 2, 2, 1, 1> zzyy, bbgg, pptt;
			detail::Swizzle<T, 4, 3, 2, 1, 1> wzyy, abgg, qptt;
			detail::Swizzle<T, 4, 0, 3, 1, 1> xwyy, ragg, sqtt;
			detail::Swizzle<T, 4, 1, 3, 1, 1> ywyy, gagg, tqtt;
			detail::Swizzle<T, 4, 2, 3, 1, 1> zwyy, bagg, pqtt;
			detail::Swizzle<T, 4, 3, 3, 1, 1> wwyy, aagg, qqtt;
			detail::Swizzle<T, 4, 0, 0, 2, 1> xxzy, rrbg, sspt;
			detail::Swizzle<T, 4, 1, 0, 2, 1> yxzy, grbg, tspt;
			detail::Swizzle<T, 4, 2, 0, 2, 1> zxzy, brbg, pspt;
			detail::Swizzle<T, 4, 3, 0, 2, 1> wxzy, arbg, qspt;
			detail::Swizzle<T, 4, 0, 1, 2, 1> xyzy, rgbg, stpt;
			detail::Swizzle<T, 4, 1, 1, 2, 1> yyzy, ggbg, ttpt;
			detail::Swizzle<T, 4, 2, 1, 2, 1> zyzy, bgbg, ptpt;
			detail::Swizzle<T, 4, 3, 1, 2, 1> wyzy, agbg, qtpt;
			detail::Swizzle<T, 4, 0, 2, 2, 1> xzzy, rbbg, sppt;
			detail::Swizzle<T, 4, 1, 2, 2, 1> yzzy, gbbg, tppt;
			detail::Swizzle<T, 4, 2, 2, 2, 1> zzzy, bbbg, pppt;
			detail::Swizzle<T, 4, 3, 2, 2, 1> wzzy, abbg, qppt;
			detail::Swizzle<T, 4, 0, 3, 2, 1> xwzy, rabg, sqpt;
			detail::Swizzle<T, 4, 1, 3, 2, 1> ywzy, gabg, tqpt;
			detail::Swizzle<T, 4, 2, 3, 2, 1> zwzy, babg, pqpt;
			detail::Swizzle<T, 4, 3, 3, 2, 1> wwzy, aabg, qqpt;
			detail::Swizzle<T, 4, 0, 0, 3, 1> xxwy, rrag, ssqt;
			detail::Swizzle<T, 4, 1, 0, 3, 1> yxwy, grag, tsqt;
			detail::Swizzle<T, 4, 2, 0, 3, 1> zxwy, brag, psqt;
			detail::Swizzle<T, 4, 3, 0, 3, 1> wxwy, arag, qsqt;
			detail::Swizzle<T, 4, 0, 1, 3, 1> xywy, rgag, stqt;
			detail::Swizzle<T, 4, 1, 1, 3, 1> yywy, ggag, ttqt;
			detail::Swizzle<T, 4, 2, 1, 3, 1> zywy, bgag, ptqt;
			detail::Swizzle<T, 4, 3, 1, 3, 1> wywy, agag, qtqt;
			detail::Swizzle<T, 4, 0, 2, 3, 1> xzwy, rbag, spqt;
			detail::Swizzle<T, 4, 1, 2, 3, 1> yzwy, gbag, tpqt;
			detail::Swizzle<T, 4, 2, 2, 3, 1> zzwy, bbag, ppqt;
			detail::Swizzle<T, 4, 3, 2, 3, 1> wzwy, abag, qpqt;
			detail::Swizzle<T, 4, 0, 3, 3, 1> xwwy, raag, sqqt;
			detail::Swizzle<T, 4, 1, 3, 3, 1> ywwy, gaag, tqqt;
			detail::Swizzle<T, 4, 2, 3, 3, 1> zwwy, baag, pqqt;
			detail::Swizzle<T, 4, 3, 3, 3, 1> wwwy, aaag, qqqt;
			detail::Swizzle<T, 4, 0, 0, 0, 2> xxxz, rrrb, sssp;
			detail::Swizzle<T, 4, 1, 0, 0, 2> yxxz, grrb, tssp;
			detail::Swizzle<T, 4, 2, 0, 0, 2> zxxz, brrb, pssp;
			detail::Swizzle<T, 4, 3, 0, 0, 2> wxxz, arrb, qssp;
			detail::Swizzle<T, 4, 0, 1, 0, 2> xyxz, rgrb, stsp;
			detail::Swizzle<T, 4, 1, 1, 0, 2> yyxz, ggrb, ttsp;
			detail::Swizzle<T, 4, 2, 1, 0, 2> zyxz, bgrb, ptsp;
			detail::Swizzle<T, 4, 3, 1, 0, 2> wyxz, agrb, qtsp;
			detail::Swizzle<T, 4, 0, 2, 0, 2> xzxz, rbrb, spsp;
			detail::Swizzle<T, 4, 1, 2, 0, 2> yzxz, gbrb, tpsp;
			detail::Swizzle<T, 4, 2, 2, 0, 2> zzxz, bbrb, ppsp;
			detail::Swizzle<T, 4, 3, 2, 0, 2> wzxz, abrb, qpsp;
			detail::Swizzle<T, 4, 0, 3, 0, 2> xwxz, rarb, sqsp;
			detail::Swizzle<T, 4, 1, 3, 0, 2> ywxz, garb, tqsp;
			detail::Swizzle<T, 4, 2, 3, 0, 2> zwxz, barb, pqsp;
			detail::Swizzle<T, 4, 3, 3, 0, 2> wwxz, aarb, qqsp;
			detail::Swizzle<T, 4, 0, 0, 1, 2> xxyz, rrgb, sstp;
			detail::Swizzle<T, 4, 1, 0, 1, 2> yxyz, grgb, tstp;
			detail::Swizzle<T, 4, 2, 0, 1, 2> zxyz, brgb, pstp;
			detail::Swizzle<T, 4, 3, 0, 1, 2> wxyz, argb, qstp;
			detail::Swizzle<T, 4, 0, 1, 1, 2> xyyz, rggb, sttp;
			detail::Swizzle<T, 4, 1, 1, 1, 2> yyyz, gggb, tttp;
			detail::Swizzle<T, 4, 2, 1, 1, 2> zyyz, bggb, pttp;
			detail::Swizzle<T, 4, 3, 1, 1, 2> wyyz, aggb, qttp;
			detail::Swizzle<T, 4, 0, 2, 1, 2> xzyz, rbgb, sptp;
			detail::Swizzle<T, 4, 1, 2, 1, 2> yzyz, gbgb, tptp;
			detail::Swizzle<T, 4, 2, 2, 1, 2> zzyz, bbgb, pptp;
			detail::Swizzle<T, 4, 3, 2, 1, 2> wzyz, abgb, qptp;
			detail::Swizzle<T, 4, 0, 3, 1, 2> xwyz, ragb, sqtp;
			detail::Swizzle<T, 4, 1, 3, 1, 2> ywyz, gagb, tqtp;
			detail::Swizzle<T, 4, 2, 3, 1, 2> zwyz, bagb, pqtp;
			detail::Swizzle<T, 4, 3, 3, 1, 2> wwyz, aagb, qqtp;
			detail::Swizzle<T, 4, 0, 0, 2, 2> xxzz, rrbb, sspp;
			detail::Swizzle<T, 4, 1, 0, 2, 2> yxzz, grbb, tspp;
			detail::Swizzle<T, 4, 2, 0, 2, 2> zxzz, brbb, pspp;
			detail::Swizzle<T, 4, 3, 0, 2, 2> wxzz, arbb, qspp;
			detail::Swizzle<T, 4, 0, 1, 2, 2> xyzz, rgbb, stpp;
			detail::Swizzle<T, 4, 1, 1, 2, 2> yyzz, ggbb, ttpp;
			detail::Swizzle<T, 4, 2, 1, 2, 2> zyzz, bgbb, ptpp;
			detail::Swizzle<T, 4, 3, 1, 2, 2> wyzz, agbb, qtpp;
			detail::Swizzle<T, 4, 0, 2, 2, 2> xzzz, rbbb, sppp;
			detail::Swizzle<T, 4, 1, 2, 2, 2> yzzz, gbbb, tppp;
			detail::Swizzle<T, 4, 2, 2, 2, 2> zzzz, bbbb, pppp;
			detail::Swizzle<T, 4, 3, 2, 2, 2> wzzz, abbb, qppp;
			detail::Swizzle<T, 4, 0, 3, 2, 2> xwzz, rabb, sqpp;
			detail::Swizzle<T, 4, 1, 3, 2, 2> ywzz, gabb, tqpp;
			detail::Swizzle<T, 4, 2, 3, 2, 2> zwzz, babb, pqpp;
			detail::Swizzle<T, 4, 3, 3, 2, 2> wwzz, aabb, qqpp;
			detail::Swizzle<T, 4, 0, 0, 3, 2> xxwz, rrab, ssqp;
			detail::Swizzle<T, 4, 1, 0, 3, 2> yxwz, grab, tsqp;
			detail::Swizzle<T, 4, 2, 0, 3, 2> zxwz, brab, psqp;
			detail::Swizzle<T, 4, 3, 0, 3, 2> wxwz, arab, qsqp;
			detail::Swizzle<T, 4, 0, 1, 3, 2> xywz, rgab, stqp;
			detail::Swizzle<T, 4, 1, 1, 3, 2> yywz, ggab, ttqp;
			detail::Swizzle<T, 4, 2, 1, 3, 2> zywz, bgab, ptqp;
			detail::Swizzle<T, 4, 3, 1, 3, 2> wywz, agab, qtqp;
			detail::Swizzle<T, 4, 0, 2, 3, 2> xzwz, rbab, spqp;
			detail::Swizzle<T, 4, 1, 2, 3, 2> yzwz, gbab, tpqp;
			detail::Swizzle<T, 4, 2, 2, 3, 2> zzwz, bbab, ppqp;
			detail::Swizzle<T, 4, 3, 2, 3, 2> wzwz, abab, qpqp;
			detail::Swizzle<T, 4, 0, 3, 3, 2> xwwz, raab, sqqp;
			detail::Swizzle<T, 4, 1, 3, 3, 2> ywwz, gaab, tqqp;
			detail::Swizzle<T, 4, 2, 3, 3, 2> zwwz, baab, pqqp;
			detail::Swizzle<T, 4, 3, 3, 3, 2> wwwz, aaab, qqqp;
			detail::Swizzle<T, 4, 0, 0, 0, 3> xxxw, rrra, sssq;
			detail::Swizzle<T, 4, 1, 0, 0, 3> yxxw, grra, tssq;
			detail::Swizzle<T, 4, 2, 0, 0, 3> zxxw, brra, pssq;
			detail::Swizzle<T, 4, 3, 0, 0, 3> wxxw, arra, qssq;
			detail::Swizzle<T, 4, 0, 1, 0, 3> xyxw, rgra, stsq;
			detail::Swizzle<T, 4, 1, 1, 0, 3> yyxw, ggra, ttsq;
			detail::Swizzle<T, 4, 2, 1, 0, 3> zyxw, bgra, ptsq;
			detail::Swizzle<T, 4, 3, 1, 0, 3> wyxw, agra, qtsq;
			detail::Swizzle<T, 4, 0, 2, 0, 3> xzxw, rbra, spsq;
			detail::Swizzle<T, 4, 1, 2, 0, 3> yzxw, gbra, tpsq;
			detail::Swizzle<T, 4, 2, 2, 0, 3> zzxw, bbra, ppsq;
			detail::Swizzle<T, 4, 3, 2, 0, 3> wzxw, abra, qpsq;
			detail::Swizzle<T, 4, 0, 3, 0, 3> xwxw, rara, sqsq;
			detail::Swizzle<T, 4, 1, 3, 0, 3> ywxw, gara, tqsq;
			detail::Swizzle<T, 4, 2, 3, 0, 3> zwxw, bara, pqsq;
			detail::Swizzle<T, 4, 3, 3, 0, 3> wwxw, aara, qqsq;
			detail::Swizzle<T, 4, 0, 0, 1, 3> xxyw, rrga, sstq;
			detail::Swizzle<T, 4, 1, 0, 1, 3> yxyw, grga, tstq;
			detail::Swizzle<T, 4, 2, 0, 1, 3> zxyw, brga, pstq;
			detail::Swizzle<T, 4, 3, 0, 1, 3> wxyw, arga, qstq;
			detail::Swizzle<T, 4, 0, 1, 1, 3> xyyw, rgga, sttq;
			detail::Swizzle<T, 4, 1, 1, 1, 3> yyyw, ggga, tttq;
			detail::Swizzle<T, 4, 2, 1, 1, 3> zyyw, bgga, pttq;
			detail::Swizzle<T, 4, 3, 1, 1, 3> wyyw, agga, qttq;
			detail::Swizzle<T, 4, 0, 2, 1, 3> xzyw, rbga, sptq;
			detail::Swizzle<T, 4, 1, 2, 1, 3> yzyw, gbga, tptq;
			detail::Swizzle<T, 4, 2, 2, 1, 3> zzyw, bbga, pptq;
			detail::Swizzle<T, 4, 3, 2, 1, 3> wzyw, abga, qptq;
			detail::Swizzle<T, 4, 0, 3, 1, 3> xwyw, raga, sqtq;
			detail::Swizzle<T, 4, 1, 3, 1, 3> ywyw, gaga, tqtq;
			detail::Swizzle<T, 4, 2, 3, 1, 3> zwyw, baga, pqtq;
			detail::Swizzle<T, 4, 3, 3, 1, 3> wwyw, aaga, qqtq;
			detail::Swizzle<T, 4, 0, 0, 2, 3> xxzw, rrba, sspq;
			detail::Swizzle<T, 4, 1, 0, 2, 3> yxzw, grba, tspq;
			detail::Swizzle<T, 4, 2, 0, 2, 3> zxzw, brba, pspq;
			detail::Swizzle<T, 4, 3, 0, 2, 3> wxzw, arba, qspq;
			detail::Swizzle<T, 4, 0, 1, 2, 3> xyzw, rgba, stpq;
			detail::Swizzle<T, 4, 1, 1, 2, 3> yyzw, ggba, ttpq;
			detail::Swizzle<T, 4, 2, 1, 2, 3> zyzw, bgba, ptpq;
			detail::Swizzle<T, 4, 3, 1, 2, 3> wyzw, agba, qtpq;
			detail::Swizzle<T, 4, 0, 2, 2, 3> xzzw, rbba, sppq;
			detail::Swizzle<T, 4, 1, 2, 2, 3> yzzw, gbba, tppq;
			detail::Swizzle<T, 4, 2, 2, 2, 3> zzzw, bbba, pppq;
			detail::Swizzle<T, 4, 3, 2, 2, 3> wzzw, abba, qppq;
			detail::Swizzle<T, 4, 0, 3, 2, 3> xwzw, raba, sqpq;
			detail::Swizzle<T, 4, 1, 3, 2, 3> ywzw, gaba, tqpq;
			detail::Swizzle<T, 4, 2, 3, 2, 3> zwzw, baba, pqpq;
			detail::Swizzle<T, 4, 3, 3, 2, 3> wwzw, aaba, qqpq;
			detail::Swizzle<T, 4, 0, 0, 3, 3> xxww, rraa, ssqq;
			detail::Swizzle<T, 4, 1, 0, 3, 3> yxww, graa, tsqq;
			detail::Swizzle<T, 4, 2, 0, 3, 3> zxww, braa, psqq;
			detail::Swizzle<T, 4, 3, 0, 3, 3> wxww, araa, qsqq;
			detail::Swizzle<T, 4, 0, 1, 3, 3> xyww, rgaa, stqq;
			detail::Swizzle<T, 4, 1, 1, 3, 3> yyww, ggaa, ttqq;
			detail::Swizzle<T, 4, 2, 1, 3, 3> zyww, bgaa, ptqq;
			detail::Swizzle<T, 4, 3, 1, 3, 3> wyww, agaa, qtqq;
			detail::Swizzle<T, 4, 0, 2, 3, 3> xzww, rbaa, spqq;
			detail::Swizzle<T, 4, 1, 2, 3, 3> yzww, gbaa, tpqq;
			detail::Swizzle<T, 4, 2, 2, 3, 3> zzww, bbaa, ppqq;
			detail::Swizzle<T, 4, 3, 2, 3, 3> wzww, abaa, qpqq;
			detail::Swizzle<T, 4, 0, 3, 3, 3> xwww, raaa, sqqq;
			detail::Swizzle<T, 4, 1, 3, 3, 3> ywww, gaaa, tqqq;
			detail::Swizzle<T, 4, 2, 3, 3, 3> zwww, baaa, pqqq;
			detail::Swizzle<T, 4, 3, 3, 3, 3> wwww, aaaa, qqqq;
#endif
		};
	};

	//Dot, (Cross?), Abs, Min, Max, Floor, Ceil
	template<Numeric T>
	constexpr inline T Dot(const vec<4, T>& a, const vec<4, T>& b) {
		return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	}

	template<Numeric T>
	constexpr inline vec<4, T> Abs(const vec<4, T>& v) {
		return vec<4, T>(Abs(v.x), Abs(v.y), Abs(v.z), Abs(v.w));
	}

	template<Numeric T>
	constexpr inline vec<4, T> Min(const vec<4, T>& v) {
		return vec<4, T>(Min(v.x), Min(v.y), Min(v.z), Min(v.w));
	}

	template<Numeric T>
	constexpr inline vec<4, T> Max(const vec<4, T>& v) {
		return vec<4, T>(Max(v.x), Max(v.y), Max(v.z), Max(v.w));
	}

	template<Numeric T>
	constexpr inline vec<4, T> Floor(const vec<4, T>& v) {
		return vec<4, T>(Floor(v.x), Floor(v.y), Floor(v.z), Floor(v.w));
	}

	template<Numeric T>
	constexpr inline vec<4, T> Ceil(const vec<4, T>& v) {
		return vec<4, T>(Ceil(v.x), Ceil(v.y), Ceil(v.z), Ceil(v.w));
	}

	typedef vec<4, f32>  vec4;
	typedef vec<4, f64> dvec4;
	typedef vec<4, i32> ivec4;
	typedef vec<4, u32> uvec4;
	typedef vec<4, b8>  bvec4;
}

template<math::Numeric T>
constexpr inline math::vec<4, T> operator+(const math::vec<4, T>& a, const math::vec<4, T>& b) {
	return math::vec<4, T>(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

template<math::Numeric T>
constexpr inline math::vec<4, T> operator-(const math::vec<4, T>& a, const math::vec<4, T>& b) {
	return math::vec<4, T>(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

template<math::Numeric T>
constexpr inline math::vec<4, T> operator*(const math::vec<4, T>& a, const math::vec<4, T>& b) {
	return math::vec<4, T>(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
}

template<math::Numeric T>
constexpr inline math::vec<4, T> operator/(const math::vec<4, T>& a, const math::vec<4, T>& b) {
	return math::vec<4, T>(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);
}

template<math::Numeric T>
constexpr inline math::vec<4, T> operator%(const math::vec<4, T>& a, const math::vec<4, T>& b) {
	return math::vec<4, T>(a.x % b.x, a.y % b.y, a.z % b.z, a.w % b.w);
}

template<math::Numeric T>
constexpr inline math::vec<4, T> operator*(const math::vec<4, T>& v, T s) {
	return math::vec<4, T>(v.x * s, v.y * s, v.z * s, v.w * s);
}

template<math::Numeric T>
constexpr inline math::vec<4, T> operator/(const math::vec<4, T>& v, T s) {
	const typename types::IntToFloat<T>::Type invS = 1.0 / s;
	return math::vec<4, T>(v.x * invS, v.y * invS, v.z * invS, v.w * invS);
}