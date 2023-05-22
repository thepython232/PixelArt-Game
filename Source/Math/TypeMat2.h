#pragma once

#include "Common.h"
#include "TypeMat.h"

namespace math {

	template<Numeric T>
	struct mat<2, 2, T> {
	public:
		using Type = mat<2, 2, T>;
		using DataType = T;
		using ColumnType = vec<2, T>;
		using RowType = vec<2, T>;
		static constexpr usize Columns = 2;
		static constexpr usize Row = 2;

		static const mat<2, 2, T> Zero;
		static const mat<2, 2, T> Identity;

		constexpr mat() = default;

		constexpr explicit mat(T s) {
			e[0][0] = s;
			e[1][1] = s;
		}

		constexpr mat(T a, T b) {
			e[0][0] = a;
			e[1][1] = b;
		}

		constexpr mat(T e00, T e01, T e10, T e11) {
			e[0] = vec<2, T>(e00, e01);
			e[1] = vec<2, T>(e10, e11);
		}

		constexpr mat(const vec<2, T>& a, const vec<2, T>& b) {
			e[0] = a;
			e[1] = b;
		}

		template<Numeric U>
		constexpr explicit mat(const mat<2, 2, U>& other) {
			e[0] = static_cast<vec<2, T>>(other[0]);
			e[1] = static_cast<vec<2, T>>(other[1]);
		}

		constexpr inline vec<2, T>& operator[](usize i) {
			ASSERT(i >= 0 && i < 2, "mat2 subscript out of range!");
			return e[i];
		}

		constexpr inline const vec<2, T>& operator[](usize i) const {
			ASSERT(i >= 0 && i < 2, "mat2 subscript out of range!");
			return e[i];
		}

		constexpr inline bool operator==(const mat<2, 2, T>& other) const {
			return e[0] == other.e[0] && e[1] == other.e[1];
		}

		constexpr inline bool operator!=(const mat<2, 2, T>& other) const {
			return e[0] != other.e[0] || e[1] != other.e[1];
		}

		constexpr inline void operator+=(const mat<2, 2, T>& other) {
			e[0] += other[0];
			e[1] += other[1];
		}

		constexpr inline void operator-=(const mat<2, 2, T>& other) {
			e[0] -= other[0];
			e[1] -= other[1];
		}

		constexpr inline void operator*=(T s) {
			e[0] *= s;
			e[1] *= s;
		}

		constexpr inline void operator/=(T s) {
			e[0] /= s;
			e[1] /= s;
		}

		constexpr inline void operator*=(const mat<2, 2, T>& other) {
			*this = other * (*this);
		}

		constexpr inline mat<2, 2, T> operator-() const {
			return mat<2, 2, T>(-e[0], -e[1]);
		}

		constexpr inline mat<2, 2, T> operator+() const {
			return *this;
		}

	private:
		vec<2, T> e[2];
	};

	template<Numeric T>
	const mat<2, 2, T> mat<2, 2, T>::Zero = mat<2, 2, T>(0.f);

	template<Numeric T>
	const mat<2, 2, T> mat<2, 2, T>::Identity = mat<2, 2, T>(1.f);

	template<Numeric T>
	constexpr inline mat<2, 2, T> Transpose(const mat<2, 2, T>& m) {
		return mat<2, 2, T>(m[0][0], m[1][0], m[0][1], m[1][1]);
	}

	template<Numeric T>
	constexpr inline T Minor(const mat<2, 2, T>& m, const uvec2& p) {
		return m[p.x ^ 1][p.y ^ 1];
	}

	template<Numeric T>
	constexpr inline T Determinant(const mat<2, 2, T>& m) {
		return m[0][0] * m[1][1] - m[0][1] * m[1][0];
	}

	template<Numeric T>
	constexpr inline mat<2, 2, T> Cofactor(const mat<2, 2, T>& m) {
		return mat<2, 2, T>(
			m[1][1], -m[1][0],
			-m[0][1], m[0][0]
			);
	}

	typedef mat<2, 2, f32>  mat2;
	typedef mat<2, 2, f64> dmat2;
	typedef mat<2, 2, i32> imat2;
	typedef mat<2, 2, u32> umat2;
	typedef mat<2, 2, b8>  bmat2;
}

template<math::Numeric T>
constexpr inline math::mat<2, 2, T> operator+(const math::mat<2, 2, T>& a, const math::mat<2, 2, T>& b) {
	return math::mat<2, 2, T>(a[0] + b[0], a[1] + b[1]);
}

template<math::Numeric T>
constexpr inline math::mat<2, 2, T> operator-(const math::mat<2, 2, T>& a, const math::mat<2, 2, T>& b) {
	return math::mat<2, 2, T>(a[0] - b[0], a[1] - b[1]);
}

template<math::Numeric T>
constexpr inline math::mat<2, 2, T> operator*(const math::mat<2, 2, T>& m, T s) {
	return math::mat<2, 2, T>(m[0] * s, m[1] * s);
}

template<math::Numeric T>
constexpr inline math::mat<2, 2, T> operator/(const math::mat<2, 2, T>& m, T s) {
	return math::mat<2, 2, T>(m[0] / s, m[1] / s);
}

template<math::Numeric T>
constexpr inline math::mat<4, 2, T> operator*(const math::mat<2, 2, T>& a, const math::mat<4, 2, T>& b) {
	return math::mat<4, 2, T>(
		a[0][0] * b[0][0] + a[1][0] * b[0][1],
		a[0][1] * b[0][0] + a[1][1] * b[0][1],
		a[0][0] * b[1][0] + a[1][0] * b[1][1],
		a[0][1] * b[1][0] + a[1][1] * b[1][1],
		a[0][0] * b[2][0] + a[1][0] * b[2][1],
		a[0][1] * b[2][0] + a[1][1] * b[2][1],
		a[0][0] * b[3][0] + a[1][0] * b[3][1],
		a[0][1] * b[3][0] + a[1][1] * b[3][1]
		);
}

template<math::Numeric T>
constexpr inline math::mat<3, 2, T> operator*(const math::mat<2, 2, T>& a, const math::mat<3, 2, T>& b) {
	return math::mat<3, 2, T>(
		a[0][0] * b[0][0] + a[1][0] * b[0][1],
		a[0][1] * b[0][0] + a[1][1] * b[0][1],
		a[0][0] * b[1][0] + a[1][0] * b[1][1],
		a[0][1] * b[1][0] + a[1][1] * b[1][1],
		a[0][0] * b[2][0] + a[1][0] * b[2][1],
		a[0][1] * b[2][0] + a[1][1] * b[2][1]
		);
}

template<math::Numeric T>
constexpr inline math::mat<2, 2, T> operator*(const math::mat<2, 2, T>& a, const math::mat<2, 2, T>& b) {
	return math::mat<2, 2, T>(
		a[0][0] * b[0][0] + a[1][0] * b[0][1],
		a[0][1] * b[0][0] + a[1][1] * b[0][1],
		a[0][0] * b[1][0] + a[1][0] * b[1][1],
		a[0][1] * b[1][0] + a[1][1] * b[1][1]
		);
}

template<math::Numeric T>
constexpr inline math::vec<2, T> operator*(const math::mat<2, 2, T>& m, const math::vec<2, T>& v) {
	return math::vec<2, T>(
		m[0][0] * v[0] + m[1][0] * v[1],
		m[0][1] * v[0] + m[1][1] * v[1]
		);
}