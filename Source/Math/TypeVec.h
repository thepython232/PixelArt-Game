#pragma once

#include "Common.h"

namespace math {

	template<usize L, Numeric T>
	struct vec {
		using Type = vec<L, T>;
		using DataType = T;
		static constexpr usize Length = L;

		constexpr vec() = default;
		constexpr explicit vec(T s) {
			for (usize i = 0; i < L; i++) {
				e[i] = s;
			}
		}

		template<typename... Ts>
			requires ((std::is_convertible_v<Ts, T> && ...) && sizeof...(Ts) == L)
		constexpr vec(Ts... ts) {
			usize i = 0;
			([&i] {
				e[i++] = ts;
				}, ...);
		}

		constexpr vec(const vec<L, T>& other) {
			for (usize i = 0; i < L; i++) {
				e[i] = other[i];
			}
		}

		template<typename U>
		constexpr explicit vec(const vec<L, U>& other) {
			for (usize i = 0; i < L; i++) {
				e[i] = static_cast<T>(other[i]);
			}
		}

		constexpr inline T& operator[](usize i) {
			ASSERT(i >= 0 && i < L, "vec subscript out of range!");
			return e[i];
		}

		constexpr inline const T& operator[](usize i) const {
			ASSERT(i >= 0 && i < L, "vec subscript out of range!");
			return e[i];
		}

		constexpr inline bool operator==(const vec<L, T>& other) const {
			for (usize i = 0; i < L; i++) {
				if (e[i] != other[i]) {
					return false;
				}
			}

			return true;
		}

		constexpr inline bool operator!=(const vec<L, T>& other) const {
			for (usize i = 0; i < L; i++) {
				if (e[i] != other[i]) {
					return true;
				}
			}

			return false;
		}

		constexpr void operator+=(const vec<L, T>& other) {
			for (usize i = 0; i < L; i++) {
				e[i] += other[i];
			}
		}

		constexpr void operator-=(const vec<L, T>& other) {
			for (usize i = 0; i < L; i++) {
				e[i] -= other[i];
			}
		}

		constexpr void operator*=(const vec<L, T>& other) {
			for (usize i = 0; i < L; i++) {
				e[i] *= other[i];
			}
		}

		constexpr void operator/=(const vec<L, T>& other) {
			for (usize i = 0; i < L; i++) {
				e[i] /= other[i];
			}
		}

		constexpr void operator*=(T s) {
			for (usize i = 0; i < L; i++) {
				e[i] *= s;
			}
		}

		constexpr void operator/=(T s) {
			const typename types::IntToFloat<T>::Type invT = 1.0 / s;
			for (usize i = 0; i < L; i++) {
				e[i] *= invT;
			}
		}

		constexpr inline vec<L, T> operator-() const {
			vec<L, T> ret;
			for (usize i = 0; i < L; i++) {
				ret[i] = -e[i];
			}
			return ret;
		}

		constexpr inline vec<L, T> operator+() const { return *this; }

	private:
		T e[L];
	};

	template<usize L, Numeric T>
	constexpr inline vec<L, T> Min(const vec<L, T>& a, const vec<L, T>& b) {
		vec<L, T> ret;
		for (usize i = 0; i < L; i++) {
			ret[i] = Min(a[i], b[i]);
		}

		return ret;
	}

	template<usize L, Numeric T>
	constexpr inline vec<L, T> Max(const vec<L, T>& a, const vec<L, T>& b) {
		vec<L, T> ret;
		for (usize i = 0; i < L; i++) {
			ret[i] = Max(a[i], b[i]);
		}

		return ret;
	}

	template<usize L, Numeric T>
	constexpr inline T Dot(const vec<L, T>& a, const vec<L, T>& b) {
		T sum = (T)0;
		for (usize i = 0; i < L; i++) {
			sum += a[i] * b[i];
		}

		return sum;
	}

	template<usize L, Numeric T>
	constexpr inline vec<L, T> Abs(const vec<L, T>& v) {
		vec<L, T> ret;
		for (usize i = 0; i < L; i++) {
			ret[i] = Abs(v[i]);
		}

		return ret;
	}

	template<usize L, Numeric T>
	constexpr inline vec<L, T> Floor(const vec<L, T>& v) {
		vec<L, T> ret;
		for (usize i = 0; i < L; i++) {
			ret[i] = Floor(v[i]);
		}

		return ret;
	}

	template<usize L, Numeric T>
	constexpr inline vec<L, T> Ceil(const vec<L, T>& v) {
		vec<L, T> ret;
		for (usize i = 0; i < L; i++) {
			ret[i] = Ceil(v[i]);
		}

		return ret;
	}

	template<usize L, Numeric T>
	constexpr inline T LengthSquared(const vec<L, T>& v) {
		return Dot(v, v);
	}

	template<usize L, Numeric T>
	constexpr inline T Length(const vec<L, T>& v) {
		return Sqrt(LengthSquared(v));
	}

	template<usize L, Numeric T>
	constexpr inline T AbsDot(const vec<L, T>& a, const vec<L, T>& b) {
		return Abs(Dot(a, b));
	}

	template<usize L, Numeric T>
	constexpr inline vec<L, T> Lerp(const vec<L, T>& a, const vec<L, T>& b, f32 t) {
		return (1 - t) * a + t * b;
	}

	template<usize L, Numeric T>
	constexpr inline f32 DistanceSquared(const vec<L, T>& a, const vec<L, T>& b) {
		return LengthSquared(a - b);
	}

	template<usize L, Numeric T>
	constexpr inline f32 Distance(const vec<L, T>& a, const vec<L, T>& b) {
		return Length(a - b);
	}

	template<usize L, Numeric T>
	constexpr inline vec<L, T> Normalize(const vec<L, T>& v) {
		return v / Length(v);
	}
}

template<usize L, typename T>
constexpr inline math::vec<L, T> operator+(const math::vec<L, T>& a, const math::vec<L, T>& b) {
	math::vec<L, T> ret;
	for (usize i = 0; i < L; i++) {
		ret[i] = a[i] + b[i];
	}

	return ret;
}

template<usize L, typename T>
constexpr inline math::vec<L, T> operator-(const math::vec<L, T>& a, const math::vec<L, T>& b) {
	math::vec<L, T> ret;
	for (usize i = 0; i < L; i++) {
		ret[i] = a[i] - b[i];
	}

	return ret;
}

template<usize L, typename T>
constexpr inline math::vec<L, T> operator*(const math::vec<L, T>& a, const math::vec<L, T>& b) {
	math::vec<L, T> ret;
	for (usize i = 0; i < L; i++) {
		ret[i] = a[i] * b[i];
	}

	return ret;
}

template<usize L, typename T>
constexpr inline math::vec<L, T> operator/(const math::vec<L, T>& a, const math::vec<L, T>& b) {
	math::vec<L, T> ret;
	for (usize i = 0; i < L; i++) {
		ret[i] = a[i] / b[i];
	}

	return ret;
}

template<usize L, typename T>
constexpr inline math::vec<L, T> operator*(const math::vec<L, T>& v, T s) {
	math::vec<L, T> ret;
	for (usize i = 0; i < L; i++) {
		ret[i] = v[i] * s;
	}

	return ret;
}

template<usize L, typename T>
constexpr inline math::vec<L, T> operator/(const math::vec<L, T>& v, T s) {
	const typename types::IntToFloat<T>::Type invS = 1.0 / s;
	math::vec<L, T> ret;
	for (usize i = 0; i < L; i++) {
		ret[i] = v[i] * invS;
	}

	return ret;
}

template<usize L, typename T>
constexpr inline math::vec<L, T> operator%(const math::vec<L, T>& a, const math::vec<L, T>& b) {
	math::vec<L, T> ret;
	for (usize i = 0; i < L; i++) {
		ret[i] = a % b;
	}

	return ret;
}