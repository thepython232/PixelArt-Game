#pragma once

#include "Common.h"
#include "TypeVec.h"

namespace math {

	template<usize C, usize R, Numeric T>
	struct mat {
		using Type = mat<C, R, T>;
		using DataType = T;
		using ColumnType = vec<R, T>;
		using RowType = vec<C, T>;
		static constexpr usize Columns = C;
		static constexpr usize Row = R;

		static const mat<C, R, T> Zero;

		constexpr mat() = default;

		template<typename... Ts>
			requires ((std::is_same_v<typename std::decay<Ts>::type, vec<R, T>> && ...) && sizeof...(Ts) == C)
		constexpr mat(Ts&&... cs) {
			usize i = 0;
			([] {
				e[i++] = std::forward<Ts>(cs);
				} (), ...);
		}

		template<typename... Ts>
			requires ((std::is_convertible_v<Ts, T> && ...) && sizeof...(Ts) == C * R)
		constexpr mat(Ts... ts) {
			usize i = 0;
			([] {
				e[i / R][i % R] = ts;
				i++;
				} (), ...);
		}

		template<Numeric U>
		constexpr explicit mat(const mat<C, R, U>& other) {
			for (usize i = 0; i < R; i++) {
				e[i] = static_cast<vec<C, T>>(other[i]);
			}
		}

		constexpr inline vec<R, T>& operator[](usize i) {
			ASSERT(i >= 0 && i < C, "mat subscript out of range!");
			return e[i];
		}

		constexpr inline const vec<R, T>& operator[](usize i) const {
			ASSERT(i >= 0 && i < C, "mat subscript out of range!");
			return e[i];
		}

		constexpr inline bool operator==(const mat<C, R, T>& other) const {
			for (usize i = 0; i < C; i++) {
				if (e[i] != other[i]) {
					return false;
				}
			}

			return true;
		}

		constexpr inline bool operator!=(const mat<C, R, T>& other) const {
			for (usize i = 0; i < C; i++) {
				if (e[i] != other[i]) {
					return true;
				}
			}

			return false;
		}

		constexpr inline void operator+=(const mat<C, R, T>& other) {
			for (usize i = 0; i < C; i++) {
				e[i] += other[i];
			}
		}

		constexpr inline void operator-=(const mat<C, R, T>& other) {
			for (usize i = 0; i < C; i++) {
				e[i] -= other[i];
			}
		}

		constexpr inline void operator*=(T s) {
			for (usize i = 0; i < C; i++) {
				e[i] *= s;
			}
		}

		constexpr inline void operator/=(T s) {
			for (usize i = 0; i < C; i++) {
				e[i] /= s;
			}
		}

		constexpr inline void operator*=(const mat<R, R, T>& other) {
			*this = other * (*this);
		}

		constexpr inline mat<C, R, T> operator-() const {
			mat<C, R, T> ret;
			for (usize i = 0; i < C; i++) {
				ret[i] = -e[i];
			}

			return ret;
		}

		constexpr inline mat<C, R, T> operator+() const {
			return *this;
		}

	private:
		vec<R, T> e[C];
	};

	template<usize C, usize R, Numeric T>
	inline const mat<C, R, T> mat<C, R, T>::Zero = mat<C, R, T>();

	template<usize L, Numeric T>
	struct mat<L, L, T> {
		using Type = mat<L, L, T>;
		using DataType = T;
		using ColumnType = vec<L, T>;
		using RowType = vec<L, T>;
		static constexpr usize Columns = L;
		static constexpr usize Row = L;

		static const mat<L, L, T> Zero;
		static const mat<L, L, T> Identity;

		constexpr mat() = default;

		template<typename... Ts>
			requires ((std::is_convertible_v<typename std::decay<Ts>::type, vec<L, T>> && ...) && sizeof...(Ts) == L)
		constexpr mat(Ts&&... cs) {
			usize i = 0;
			([this, &i, &cs] {
				e[i++] = cs;
				} (), ...);
		}

		template<typename... Ts>
			requires ((std::is_convertible_v<Ts, T> && ...) && sizeof...(Ts) == L * L)
		constexpr mat(Ts... ts) {
			usize i = 0;
			([&] {
				e[i / L][i % L] = ts;
				i++;
				} (), ...);
		}

		constexpr explicit mat(T s) {
			for (usize i = 0; i < L; i++) {
				e[i][i] = s;
			}
		}

		template<typename... Ts>
			requires ((std::is_convertible_v<Ts, T> && ...) && sizeof...(Ts) == L)
		constexpr mat(Ts... ss) {
			usize i = 0;
			([] {
				e[i][i] = ss;
				i++;
				} (), ...);
		}

		template<Numeric U>
		constexpr explicit mat(const mat<L, L, U>& other) {
			for (usize i = 0; i < L; i++) {
				e[i] = static_cast<vec<L, T>>(other[i]);
			}
		}

		constexpr inline vec<L, T>& operator[](usize i) {
			ASSERT(i >= 0 && i < L, "mat subscript out of range!");
			return e[i];
		}

		constexpr inline const vec<L, T>& operator[](usize i) const {
			ASSERT(i >= 0 && i < L, "mat subscript out of range!");
			return e[i];
		}

		constexpr inline bool operator==(const mat<L, L, T>& other) const {
			for (usize i = 0; i < L; i++) {
				if (e[i] != other[i]) {
					return false;
				}
			}

			return true;
		}

		constexpr inline bool operator!=(const mat<L, L, T>& other) const {
			for (usize i = 0; i < L; i++) {
				if (e[i] != other[i]) {
					return true;
				}
			}

			return false;
		}

		constexpr inline void operator+=(const mat<L, L, T>& other) {
			for (usize i = 0; i < L; i++) {
				e[i] += other[i];
			}
		}

		constexpr inline void operator-=(const mat<L, L, T>& other) {
			for (usize i = 0; i < L; i++) {
				e[i] -= other[i];
			}
		}

		constexpr inline void operator*=(T s) {
			for (usize i = 0; i < L; i++) {
				e[i] *= s;
			}
		}

		constexpr inline void operator/=(T s) {
			for (usize i = 0; i < L; i++) {
				e[i] /= s;
			}
		}

		constexpr inline void operator*=(const mat<L, L, T>& other) {
			*this = other * (*this);
		}

		constexpr inline mat<L, L, T> operator-() const {
			mat<L, L, T> ret;
			for (usize i = 0; i < L; i++) {
				ret[i] = -e[i];
			}

			return ret;
		}

		constexpr inline mat<L, L, T> operator+() const {
			return *this;
		}

	private:
		vec<L, T> e[L];
	};

	template<usize L, Numeric T>
	inline const mat<L, L, T> mat<L, L, T>::Zero = mat<L, L, T>((T)0);

	template<usize L, Numeric T>
	inline const mat<L, L, T> mat<L, L, T>::Identity = mat<L, L, T>((T)1);

	//Common Functions

	template<usize C, usize R, Numeric T>
	constexpr inline mat<R, C, T> Transpose(const mat<C, R, T>& m) {
		mat<R, C, T> ret;
		for (usize i = 0; i < C; i++) {
			for (usize j = 0; j < R; j++) {
				ret[j][i] = m[i][j];
			}
		}

		return ret;
	}

	template<usize L, Numeric T>
	constexpr inline mat<L - 1, L - 1, T> Minor(const mat<L, L, T>& m, const uvec2& p) {
		mat<L - 1, L - 1, T> ret;
		for (usize i = 0; i < L; i++) {
			for (usize j = 0; j < L; j++) {
				if (i == p.x || j == p.y) continue;

				uvec2 pp{ i, j };
				if (i > p.x) pp.x--;
				if (j > p.y) pp.y--;

				ret[pp.x][pp.y] = m[i][j];
			}
		}

		return ret;
	}

	template<usize L, Numeric T>
	constexpr inline T Determinant(const mat<L, L, T>& m) {
		if constexpr (L == 2) {
			return m[0][0] * m[1][1] - m[0][1] * m[1][0];
		}
		else if constexpr (L == 3) {
			const mat<2, 2, T> minorA(m[1][1], m[1][2], m[2][1], m[2][2]);
			const mat<2, 2, T> minorB(m[0][1], m[0][2], m[2][1], m[2][2]);
			const mat<2, 2, T> minorC(m[0][1], m[0][2], m[1][1], m[1][2]);

			return m[0][0] * Determinant(minorA) - m[1][0] * Determinant(minorB) + m[2][0] * Determinant(minorC);
		}
		else if constexpr (L == 4) {
			const mat<3, 3, T> minorA = Minor(m, { 0, 0 });
			const mat<3, 3, T> minorB = Minor(m, { 1, 0 });
			const mat<3, 3, T> minorC = Minor(m, { 2, 0 });
			const mat<3, 3, T> minorD = Minor(m, { 3, 0 });

			return m[0][0] * Determinant(minorA) - m[1][0] * Determinant(minorB) + m[2][0] * Determinant(minorC) - m[3][0] * Determinant(minorD);
		}
	}

	template<usize L, Numeric T>
	constexpr inline mat<L, L, T> Cofactor(const mat<L, L, T>& m) {
		mat<L, L, T> ret;
		for (usize i = 0; i < L; i++) {
			for (usize j = 0; j < L; j++) {
				bool flipSign = (i + j) % 2 == 1;
				auto minor = Minor(m, uvec2{ i, j });
				ret[i][j] = Determinant(minor) * (flipSign ? -1 : 1);
			}
		}

		return ret;
	}

	template<usize L, Numeric T>
	constexpr inline mat<L, L, T> Adjugate(const mat<L, L, T>& m) {
		return Transpose(Cofactor(m));
	}
}

template<usize C, usize R, math::Numeric T>
constexpr inline math::mat<C, R, T> operator+(const math::mat<C, R, T>& a, const math::mat<C, R, T>& b) {
	math::mat<C, R, T> ret;
	for (usize i = 0; i < C; i++) {
		ret[i] = a[i] + b[i];
	}

	return ret;
}

template<usize C, usize R, math::Numeric T>
constexpr inline math::mat<C, R, T> operator-(const math::mat<C, R, T>& a, const math::mat<C, R, T>& b) {
	math::mat<C, R, T> ret;
	for (usize i = 0; i < C; i++) {
		ret[i] = a[i] - b[i];
	}

	return ret;
}

template<usize C, usize R, math::Numeric T>
constexpr inline math::mat<C, R, T> operator*(const math::mat<C, R, T>& m, T s) {
	math::mat<C, R, T> ret;
	for (usize i = 0; i < C; i++) {
		ret[i] = m[i] * s;
	}

	return ret;
}

template<usize C, usize R, math::Numeric T>
constexpr inline math::mat<C, R, T> operator/(const math::mat<C, R, T>& m, T s) {
	math::mat<C, R, T> ret;
	for (usize i = 0; i < C; i++) {
		ret[i] = m[i] / s;
	}

	return ret;
}

template<usize A, usize B, usize C, math::Numeric T>
constexpr inline math::mat<B, C, T> operator*(const math::mat<A, B, T>& a, const math::mat<C, A, T>& b) {
	math::mat<B, C, T> ret;
	auto transA = math::Transpose(a);
	for (usize i = 0; i < B; i++) {
		for (usize j = 0; j < C; j++) {
			ret[i][j] = math::Dot(transA[i], b[j]);
		}
	}

	return ret;
}

template<usize C, usize R, math::Numeric T>
constexpr inline math::vec<C, T> operator*(const math::mat<C, R, T>& m, const math::vec<C, T>& v) {
	math::vec<C, T> ret;
	auto transM = math::Transpose(m);
	for (usize i = 0; i < C; i++) {
		ret[i] = math::Dot(transM[i], v);
	}

	return ret;
}

namespace math {

	template<usize L, Numeric T>
	constexpr inline mat<L, L, T> Inverse(const mat<L, L, T>& m) {
		T det = Determinant(m);
		if (det == (T)0) {
			return mat<L, L, T>::Zero;
		}
		return Adjugate(m) / det;
	}
}