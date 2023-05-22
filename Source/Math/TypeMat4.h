#pragma once

#include "Common.h"
#include "TypeMat.h"

namespace math {

	template<Numeric T>
	struct mat<4, 4, T> {
		using Type = mat<4, 4, T>;
		using DataType = T;
		using ColumnType = vec<4, T>;
		using RowType = vec<4, T>;
		static constexpr usize Columns = 4;
		static constexpr usize Row = 4;

		static const mat<4, 4, T> Zero;
		static const mat<4, 4, T> Identity;

		constexpr mat() = default;

		constexpr mat(T s) {
			e[0][0] = s;
			e[1][1] = s;
			e[2][2] = s;
			e[3][3] = s;
		}

		constexpr mat(T a, T b, T c, T d) {
			e[0][0] = a;
			e[1][1] = b;
			e[2][2] = c;
			e[3][3] = d;
		}

		constexpr mat(T e00, T e01, T e02, T e03, T e10, T e11, T e12, T e13, T e20, T e21, T e22, T e23, T e30, T e31, T e32, T e33) {
			e[0] = vec<4, T>(e00, e01, e02, e03);
			e[1] = vec<4, T>(e10, e11, e12, e13);
			e[2] = vec<4, T>(e20, e21, e22, e23);
			e[3] = vec<4, T>(e30, e31, e32, e33);
		}

		constexpr mat(const vec<4, T>& a, const vec<4, T>& b, const vec<4, T>& c, const vec<4, T>& d) {
			e[0] = a;
			e[1] = b;
			e[2] = c;
			e[3] = d;
		}

		template<Numeric U>
		constexpr explicit mat(const mat<4, 4, U>& other) {
			e[0] = static_cast<vec<4, T>>(other[0]);
			e[1] = static_cast<vec<4, T>>(other[1]);
			e[2] = static_cast<vec<4, T>>(other[2]);
			e[3] = static_cast<vec<4, T>>(other[3]);
		}

		constexpr inline vec<4, T>& operator[](usize i) {
			ASSERT(i >= 0 && i < 4, "mat4 subscript out of range!");
			return e[i];
		}

		constexpr inline const vec<4, T>& operator[](usize i) const {
			ASSERT(i >= 0 && i < 4, "mat4 subscript out of range!");
			return e[i];
		}

		constexpr inline bool operator==(const mat<4, 4, T>& other) const {
			return e[0] == other[0] && e[1] == other[1] && e[2] == other[2] && e[3] == other[3];
		}

		constexpr inline bool operator!=(const mat<4, 4, T>& other) const {
			return e[0] != other[0] || e[1] != other[1] || e[2] != other[2] || e[3] != other[3];
		}

		constexpr inline void operator+=(const mat<4, 4, T>& other) {
			e[0] += other[0];
			e[1] += other[1];
			e[2] += other[2];
			e[3] += other[3];
		}

		constexpr inline void operator-=(const mat<4, 4, T>& other) {
			e[0] -= other[0];
			e[1] -= other[1];
			e[2] -= other[2];
			e[3] -= other[3];
		}

		constexpr inline void operator*=(T s) {
			e[0] *= s;
			e[1] *= s;
			e[2] *= s;
			e[3] *= s;
		}

		constexpr inline void operator/=(T s) {
			e[0] /= s;
			e[1] /= s;
			e[2] /= s;
			e[3] /= s;
		}

		constexpr inline void operator*=(const mat<4, 4, T>& other) {
			*this = other * (*this);
		}

		constexpr inline mat<4, 4, T> operator-() const {
			return mat<4, 4, T>(-e[0], -e[1], -e[2], -e[3]);
		}

		constexpr inline mat<4, 4, T> operator+() const {
			return *this;
		}

	private:
		vec<4, T> e[4];
	};

	template<Numeric T>
	const mat<4, 4, T> mat<4, 4, T>::Zero = mat<4, 4, T>(0);
	
	template<Numeric T>
	const mat<4, 4, T> mat<4, 4, T>::Identity = mat<4, 4, T>(1);

	template<Numeric T>
	constexpr inline mat<4, 4, T> Transpose(const mat<4, 4, T>& m) {
		return mat<4, 4, T>(
			m[0][0], m[1][0], m[2][0], m[3][0],
			m[0][1], m[1][1], m[2][1], m[3][1],
			m[0][2], m[1][2], m[2][2], m[3][2],
			m[0][3], m[1][3], m[2][3], m[3][3]
			);
	}

	template<Numeric T>
	constexpr inline mat<3, 3, T> Minor(const mat<4, 4, T>& m, const vec<2, u32>& p) {
		return mat<3, 3, T>(
			m[p.x == 0 ? 1 : 0][p.y == 0 ? 1 : 0],
			m[p.x == 0 ? 1 : 0][p.y <= 1 ? 2 : 1],
			m[p.x == 0 ? 1 : 0][p.y <= 2 ? 3 : 2],

			m[p.x <= 1 ? 2 : 1][p.y == 0 ? 1 : 0],
			m[p.x <= 1 ? 2 : 1][p.y <= 1 ? 2 : 1],
			m[p.x <= 1 ? 2 : 1][p.y <= 2 ? 3 : 2],

			m[p.x <= 2 ? 3 : 2][p.y == 0 ? 1 : 0],
			m[p.x <= 2 ? 3 : 2][p.y <= 1 ? 2 : 1],
			m[p.x <= 2 ? 3 : 2][p.y <= 2 ? 3 : 2]
			);
	}

	template<Numeric T>
	constexpr inline T Determinant(const mat<4, 4, T>& m) {
		return m[0][0] * m[1][1] * m[2][2] * m[3][3]
			+ m[1][0] * m[2][1] * m[3][2] * m[0][3]
			+ m[2][0] * m[3][1] * m[0][2] * m[1][3]
			+ m[3][0] * m[0][1] * m[1][2] * m[2][3]
			- m[0][3] * m[1][2] * m[2][1] * m[3][0]
			- m[1][3] * m[2][2] * m[3][1] * m[0][0]
			- m[2][3] * m[3][2] * m[0][1] * m[1][0]
			- m[3][3] * m[0][2] * m[1][1] * m[2][0];
	}

	template<Numeric T>
	constexpr inline mat<4, 4, T> Cofactor(const mat<4, 4, T>& m) { //Oh gosh.....
		return mat<4, 4, T>(
			m[1][1] * m[2][2] * m[3][3] + m[2][1] * m[3][2] * m[1][3] + m[3][1] * m[1][2] * m[2][3]
			- m[1][3] * m[2][2] * m[3][1] - m[2][3] * m[3][2] * m[1][1] - m[3][3] * m[1][2] * m[2][1],
			m[1][3] * m[2][2] * m[3][0] + m[2][3] * m[3][2] * m[1][0] + m[3][3] * m[1][2] * m[2][0]
			- m[1][0] * m[2][2] * m[3][3] - m[2][0] * m[3][2] * m[1][3] - m[3][0] * m[1][2] * m[2][3],
			m[1][0] * m[2][1] * m[3][3] + m[2][0] * m[3][1] * m[1][3] + m[3][0] * m[1][1] * m[2][3]
			- m[1][3] * m[2][1] * m[3][0] - m[2][3] * m[3][1] * m[1][0] - m[3][3] * m[1][1] * m[2][0],
			m[1][2] * m[2][1] * m[3][0] + m[2][2] * m[3][1] * m[1][0] + m[3][2] * m[1][1] * m[2][0]
			- m[1][0] * m[2][1] * m[3][2] - m[2][0] * m[3][1] * m[1][2] - m[3][0] * m[1][1] * m[2][2],

			m[0][3] * m[2][2] * m[3][1] + m[2][3] * m[3][2] * m[0][1] + m[3][3] * m[0][2] * m[2][1]
			- m[0][1] * m[2][2] * m[3][3] - m[2][1] * m[3][2] * m[0][3] - m[3][1] * m[0][2] * m[2][3],
			m[0][0] * m[2][2] * m[3][3] + m[2][0] * m[3][2] * m[0][3] + m[3][0] * m[0][2] * m[2][3]
			- m[0][3] * m[2][2] * m[3][0] - m[2][3] * m[3][2] * m[0][0] - m[3][3] * m[0][2] * m[2][0],
			m[0][3] * m[2][1] * m[3][0] + m[2][3] * m[3][1] * m[0][0] + m[3][3] * m[0][1] * m[2][0]
			- m[0][0] * m[2][1] * m[3][3] - m[2][0] * m[3][1] * m[0][3] - m[3][0] * m[0][1] * m[2][3],
			m[0][0] * m[2][1] * m[3][2] + m[2][0] * m[3][1] * m[0][2] + m[3][0] * m[0][1] * m[2][2]
			- m[0][2] * m[2][1] * m[3][0] - m[2][2] * m[3][1] * m[0][0] - m[3][2] * m[0][1] * m[2][0],

			m[0][1] * m[1][2] * m[3][3] + m[1][1] * m[3][2] * m[0][3] + m[3][1] * m[0][2] * m[1][3]
			- m[0][3] * m[1][2] * m[3][1] - m[1][3] * m[3][2] * m[0][1] - m[3][3] * m[0][2] * m[1][1],
			m[0][3] * m[1][2] * m[3][0] + m[1][3] * m[3][2] * m[0][0] + m[3][3] * m[0][2] * m[1][0]
			- m[0][0] * m[1][2] * m[3][3] - m[1][0] * m[3][2] * m[0][3] - m[3][0] * m[0][2] * m[1][3],
			m[0][0] * m[1][1] * m[3][3] + m[1][0] * m[3][1] * m[0][3] + m[3][0] * m[0][1] * m[1][3]
			- m[0][3] * m[1][1] * m[3][0] - m[1][3] * m[3][1] * m[0][0] - m[3][3] * m[0][1] * m[1][0],
			m[0][2] * m[1][1] * m[3][0] + m[1][2] * m[3][1] * m[0][0] + m[3][2] * m[0][1] * m[1][0]
			- m[0][0] * m[1][1] * m[3][2] - m[1][0] * m[3][1] * m[0][2] - m[3][0] * m[0][1] * m[1][2],

			m[0][3] * m[1][2] * m[2][1] + m[1][3] * m[2][2] * m[0][1] + m[2][3] * m[0][2] * m[1][1]
			- m[0][1] * m[1][2] * m[2][3] - m[1][1] * m[2][2] * m[0][3] - m[2][1] * m[0][2] * m[1][3],
			m[0][0] * m[1][2] * m[2][3] + m[1][0] * m[2][2] * m[0][3] + m[2][0] * m[0][2] * m[1][3]
			- m[0][3] * m[1][2] * m[2][0] - m[1][3] * m[2][2] * m[0][0] - m[2][3] * m[0][2] * m[1][0],
			m[0][3] * m[1][1] * m[2][0] + m[1][3] * m[2][1] * m[0][0] + m[2][3] * m[0][1] * m[1][0]
			- m[0][0] * m[1][1] * m[2][3] - m[1][0] * m[2][1] * m[0][3] - m[2][0] * m[0][1] * m[1][3],
			m[0][0] * m[1][1] * m[2][2] + m[1][0] * m[2][1] * m[0][2] + m[2][0] * m[0][1] * m[1][2]
			- m[0][2] * m[1][1] * m[2][0] - m[1][2] * m[2][1] * m[0][0] - m[2][2] * m[0][1] * m[1][0]
			);
	}

	typedef mat<4, 4, f32>  mat4;
	typedef mat<4, 4, f64> dmat4;
	typedef mat<4, 4, i32> imat4;
	typedef mat<4, 4, u32> umat4;
	typedef mat<4, 4, b8>  bmat4;
}

template<math::Numeric T>
constexpr inline math::mat<4, 4, T> operator+(const math::mat<4, 4, T>& a, const math::mat<4, 4, T>& b) {
	return math::mat<4, 4, T>(a[0] + b[0], a[1] + b[1], a[2] + b[2], a[3] + b[3]);
}

template<math::Numeric T>
constexpr inline math::mat<4, 4, T> operator-(const math::mat<4, 4, T>& a, const math::mat<4, 4, T>& b) {
	return math::mat<4, 4, T>(a[0] - b[0], a[1] - b[1], a[2] - b[2], a[3] - b[3]);
}

template<math::Numeric T>
constexpr inline math::mat<4, 4, T> operator*(const math::mat<4, 4, T>& m, T s) {
	return math::mat<4, 4, T>(m[0] * s, m[1] * s, m[2] * s, m[3] * s);
}

template<math::Numeric T>
constexpr inline math::mat<4, 4, T> operator/(const math::mat<4, 4, T>& m, T s) {
	return math::mat<4, 4, T>(m[0] / s, m[1] / s, m[2] / s, m[3] / s);
}

template<math::Numeric T>
constexpr inline math::mat<4, 4, T> operator*(const math::mat<4, 4, T>& a, const math::mat<4, 4, T>& b) {
	return math::mat<4, 4, T>(
		a[0][0] * b[0][0] + a[1][0] * b[0][1] + a[2][0] * b[0][2] + a[3][0] * b[0][3],
		a[0][1] * b[0][0] + a[1][1] * b[0][1] + a[2][1] * b[0][2] + a[3][1] * b[0][3],
		a[0][2] * b[0][0] + a[1][2] * b[0][1] + a[2][2] * b[0][2] + a[3][2] * b[0][3],
		a[0][3] * b[0][0] + a[1][3] * b[0][1] + a[2][3] * b[0][2] + a[3][3] * b[0][3],
		a[0][0] * b[1][0] + a[1][0] * b[1][1] + a[2][0] * b[1][2] + a[3][0] * b[1][3],
		a[0][1] * b[1][0] + a[1][1] * b[1][1] + a[2][1] * b[1][2] + a[3][1] * b[1][3],
		a[0][2] * b[1][0] + a[1][2] * b[1][1] + a[2][2] * b[1][2] + a[3][2] * b[1][3],
		a[0][3] * b[1][0] + a[1][3] * b[1][1] + a[2][3] * b[1][2] + a[3][3] * b[1][3],
		a[0][0] * b[2][0] + a[1][0] * b[2][1] + a[2][0] * b[2][2] + a[3][0] * b[2][3],
		a[0][1] * b[2][0] + a[1][1] * b[2][1] + a[2][1] * b[2][2] + a[3][1] * b[2][3],
		a[0][2] * b[2][0] + a[1][2] * b[2][1] + a[2][2] * b[2][2] + a[3][2] * b[2][3],
		a[0][3] * b[2][0] + a[1][3] * b[2][1] + a[2][3] * b[2][2] + a[3][3] * b[2][3],
		a[0][0] * b[3][0] + a[1][0] * b[3][1] + a[2][0] * b[3][2] + a[3][0] * b[3][3],
		a[0][1] * b[3][0] + a[1][1] * b[3][1] + a[2][1] * b[3][2] + a[3][1] * b[3][3],
		a[0][2] * b[3][0] + a[1][2] * b[3][1] + a[2][2] * b[3][2] + a[3][2] * b[3][3],
		a[0][3] * b[0][0] + a[1][3] * b[3][1] + a[2][3] * b[3][2] + a[3][3] * b[3][3]
		);
}

template<math::Numeric T>
constexpr inline math::mat<3, 4, T> operator*(const math::mat<4, 4, T>& a, const math::mat<3, 4, T>& b) {
	return math::mat<3, 4, T>(
		a[0][0] * b[0][0] + a[1][0] * b[0][1] + a[2][0] * b[0][2] + a[3][0] * b[0][3],
		a[0][1] * b[0][0] + a[1][1] * b[0][1] + a[2][1] * b[0][2] + a[3][1] * b[0][3],
		a[0][2] * b[0][0] + a[1][2] * b[0][1] + a[2][2] * b[0][2] + a[3][2] * b[0][3],
		a[0][3] * b[0][0] + a[1][3] * b[0][1] + a[2][3] * b[0][2] + a[3][3] * b[0][3],
		a[0][0] * b[1][0] + a[1][0] * b[1][1] + a[2][0] * b[1][2] + a[3][0] * b[1][3],
		a[0][1] * b[1][0] + a[1][1] * b[1][1] + a[2][1] * b[1][2] + a[3][1] * b[1][3],
		a[0][2] * b[1][0] + a[1][2] * b[1][1] + a[2][2] * b[1][2] + a[3][2] * b[1][3],
		a[0][3] * b[1][0] + a[1][3] * b[1][1] + a[2][3] * b[1][2] + a[3][3] * b[1][3],
		a[0][0] * b[2][0] + a[1][0] * b[2][1] + a[2][0] * b[2][2] + a[3][0] * b[2][3],
		a[0][1] * b[2][0] + a[1][1] * b[2][1] + a[2][1] * b[2][2] + a[3][1] * b[2][3],
		a[0][2] * b[2][0] + a[1][2] * b[2][1] + a[2][2] * b[2][2] + a[3][2] * b[2][3],
		a[0][3] * b[2][0] + a[1][3] * b[2][1] + a[2][3] * b[2][2] + a[3][3] * b[2][3]
		);
}

template<math::Numeric T>
constexpr inline math::mat<2, 4, T> operator*(const math::mat<4, 4, T>& a, const math::mat<2, 4, T>& b) {
	return math::mat<2, 4, T>(
		a[0][0] * b[0][0] + a[1][0] * b[0][1] + a[2][0] * b[0][2] + a[3][0] * b[0][3],
		a[0][1] * b[0][0] + a[1][1] * b[0][1] + a[2][1] * b[0][2] + a[3][1] * b[0][3],
		a[0][2] * b[0][0] + a[1][2] * b[0][1] + a[2][2] * b[0][2] + a[3][2] * b[0][3],
		a[0][3] * b[0][0] + a[1][3] * b[0][1] + a[2][3] * b[0][2] + a[3][3] * b[0][3],
		a[0][0] * b[1][0] + a[1][0] * b[1][1] + a[2][0] * b[1][2] + a[3][0] * b[1][3],
		a[0][1] * b[1][0] + a[1][1] * b[1][1] + a[2][1] * b[1][2] + a[3][1] * b[1][3],
		a[0][2] * b[1][0] + a[1][2] * b[1][1] + a[2][2] * b[1][2] + a[3][2] * b[1][3],
		a[0][3] * b[1][0] + a[1][3] * b[1][1] + a[2][3] * b[1][2] + a[3][3] * b[1][3]
		);
}

template<math::Numeric T>
constexpr inline math::vec<4, T> operator*(const math::mat<4, 4, T>& m, const math::vec<4, T>& v) {
	return math::vec<4, T>(
		m[0][0] * v[0] + m[1][0] * v[1] + m[2][0] * v[2] + m[3][0] * v[3],
		m[0][1] * v[0] + m[1][1] * v[1] + m[2][1] * v[2] + m[3][1] * v[3],
		m[0][2] * v[0] + m[1][2] * v[1] + m[2][2] * v[2] + m[3][2] * v[3],
		m[0][3] * v[0] + m[1][3] * v[1] + m[2][3] * v[2] + m[3][3] * v[3]
		);
}