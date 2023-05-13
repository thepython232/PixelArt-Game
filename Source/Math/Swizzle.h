#pragma once

#include "Common.h"

namespace math {

	template<usize L, Numeric T>
	struct vec;

	namespace detail {

		template<Numeric T, usize L>
		struct SwizzleBase {
		protected:
			inline T& Elem(u32 i) { return reinterpret_cast<T*>(elems)[i]; }
			inline const T& Elem(u32 i) const { return reinterpret_cast<const T*>(elems)[i]; }

		private:
			u8 elems[1];
		};

		template<Numeric T, usize L, i32 E0, i32 E1, i32 E2, i32 E3>
		struct Swizzle;

		template<Numeric T, i32 E0, i32 E1>
		struct Swizzle<T, 2, E0, E1, -1, -1> : SwizzleBase<T, 2> {
			using Base = SwizzleBase<T, 2>;

			using Type = Swizzle<T, 2, E0, E1, -1, -1>;
			using DataType = T;
			using VecType = vec<2, T>;
			static constexpr usize Length = 2;

			inline operator VecType() const {
				return VecType(Base::Elem(E0), Base::Elem(E1));
			}

			inline VecType operator()() const {
				return VecType(Base::Elem(E0), Base::Elem(E1));
			}

			Swizzle& operator=(const VecType& other) {
				Base::Elem(E0) = other[0];
				Base::Elem(E1) = other[1];

				return *this;
			}

			//TODO: remaining operators
		};

		template<Numeric T, i32 E0, i32 E1, i32 E2>
		struct Swizzle<T, 3, E0, E1, E2, -1> : SwizzleBase<T, 3> {
			using Base = SwizzleBase<T, 3>;

			using Type = Swizzle<T, 3, E0, E1, E2, -1>;
			using DataType = T;
			using VecType = vec<3, T>;
			static constexpr usize Length = 3;

			inline operator VecType() const {
				return VecType(Base::Elem(E0), Base::Elem(E1), Base::Elem(E2));
			}

			inline VecType operator()() const {
				return VecType(Base::Elem(E0), Base::Elem(E1), Base::Elem(E2));
			}

			Swizzle& operator=(const VecType& other) {
				Base::Elem(E0) = other[0];
				Base::Elem(E1) = other[1];
				Base::Elem(E2) = other[2];

				return *this;
			}

			//TODO: remaining operators
		};

		template<Numeric T, i32 E0, i32 E1, i32 E2, i32 E3>
		struct Swizzle<T, 4, E0, E1, E2, E3> : SwizzleBase<T, 4> {
			using Base = SwizzleBase<T, 4>;

			using Type = Swizzle<T, 4, E0, E1, E2, E3>;
			using DataType = T;
			using VecType = vec<4, T>;
			static constexpr usize Length = 4;

			inline operator VecType() const {
				return VecType(Base::Elem(E0), Base::Elem(E1), Base::Elem(E2), Base::Elem(E3));
			}

			inline VecType operator()() const {
				return VecType(Base::Elem(E0), Base::Elem(E1), Base::Elem(E2), Base::Elem(E3));
			}

			Swizzle& operator=(const VecType& other) {
				Base::Elem(E0) = other[0];
				Base::Elem(E1) = other[1];
				Base::Elem(E2) = other[2];
				Base::Elem(E3) = other[3];

				return *this;
			}

			//TODO: remaining operators
		};
	}
}

//Global operators with swizzle types
template<usize L, math::Numeric T, i32 E0, i32 E1, i32 E2, i32 E3>
constexpr inline math::vec<L, T> operator+(const math::vec<L, T>& a, const math::detail::Swizzle<T, L, E0, E1, E2, E3>& b) {
	return a + b();
}

template<usize L, math::Numeric T, i32 E0, i32 E1, i32 E2, i32 E3>
constexpr inline math::vec<L, T> operator+(const math::detail::Swizzle<T, L, E0, E1, E2, E3>& a, const math::vec<L, T>& b) {
	return a() + b;
}

template<usize L, math::Numeric T, i32 E0, i32 E1, i32 E2, i32 E3>
constexpr inline math::vec<L, T> operator+(const math::detail::Swizzle<T, L, E0, E1, E2, E3>& a, const math::detail::Swizzle<T, L, E0, E1, E2, E3>& b) {
	return a() + b();
}

template<usize L, math::Numeric T, i32 E0, i32 E1, i32 E2, i32 E3>
constexpr inline math::vec<L, T> operator-(const math::vec<L, T>& a, const math::detail::Swizzle<T, L, E0, E1, E2, E3>& b) {
	return a - b();
}

template<usize L, math::Numeric T, i32 E0, i32 E1, i32 E2, i32 E3>
constexpr inline math::vec<L, T> operator-(const math::detail::Swizzle<T, L, E0, E1, E2, E3>& a, const math::vec<L, T>& b) {
	return a() - b;
}

template<usize L, math::Numeric T, i32 E0, i32 E1, i32 E2, i32 E3>
constexpr inline math::vec<L, T> operator-(const math::detail::Swizzle<T, L, E0, E1, E2, E3>& a, const math::detail::Swizzle<T, L, E0, E1, E2, E3>& b) {
	return a() - b();
}

template<usize L, math::Numeric T, i32 E0, i32 E1, i32 E2, i32 E3>
constexpr inline math::vec<L, T> operator*(const math::vec<L, T>& a, const math::detail::Swizzle<T, L, E0, E1, E2, E3>& b) {
	return a * b();
}

template<usize L, math::Numeric T, i32 E0, i32 E1, i32 E2, i32 E3>
constexpr inline math::vec<L, T> operator*(const math::detail::Swizzle<T, L, E0, E1, E2, E3>& a, const math::vec<L, T>& b) {
	return a() * b;
}

template<usize L, math::Numeric T, i32 E0, i32 E1, i32 E2, i32 E3>
constexpr inline math::vec<L, T> operator*(const math::detail::Swizzle<T, L, E0, E1, E2, E3>& a, const math::detail::Swizzle<T, L, E0, E1, E2, E3>& b) {
	return a() * b();
}

template<usize L, math::Numeric T, i32 E0, i32 E1, i32 E2, i32 E3>
constexpr inline math::vec<L, T> operator/(const math::vec<L, T>& a, const math::detail::Swizzle<T, L, E0, E1, E2, E3>& b) {
	return a / b();
}

template<usize L, math::Numeric T, i32 E0, i32 E1, i32 E2, i32 E3>
constexpr inline math::vec<L, T> operator/(const math::detail::Swizzle<T, L, E0, E1, E2, E3>& a, const math::vec<L, T>& b) {
	return a() / b;
}

template<usize L, math::Numeric T, i32 E0, i32 E1, i32 E2, i32 E3>
constexpr inline math::vec<L, T> operator/(const math::detail::Swizzle<T, L, E0, E1, E2, E3>& a, const math::detail::Swizzle<T, L, E0, E1, E2, E3>& b) {
	return a() / b();
}

template<usize L, math::Numeric T, i32 E0, i32 E1, i32 E2, i32 E3>
constexpr inline math::vec<L, T> operator%(const math::vec<L, T>& a, const math::detail::Swizzle<T, L, E0, E1, E2, E3>& b) {
	return a % b();
}

template<usize L, math::Numeric T, i32 E0, i32 E1, i32 E2, i32 E3>
constexpr inline math::vec<L, T> operator%(const math::detail::Swizzle<T, L, E0, E1, E2, E3>& a, const math::vec<L, T>& b) {
	return a() % b;
}

template<usize L, math::Numeric T, i32 E0, i32 E1, i32 E2, i32 E3>
constexpr inline math::vec<L, T> operator%(const math::detail::Swizzle<T, L, E0, E1, E2, E3>& a, const math::detail::Swizzle<T, L, E0, E1, E2, E3>& b) {
	return a() % b();
}

template<usize L, math::Numeric T, i32 E0, i32 E1, i32 E2, i32 E3>
constexpr inline math::vec<L, T> operator*(const math::detail::Swizzle<T, L, E0, E1, E2, E3>& v, T s) {
	return v() * s;
}

template<usize L, math::Numeric T, i32 E0, i32 E1, i32 E2, i32 E3>
constexpr inline math::vec<L, T> operator/(const math::detail::Swizzle<T, L, E0, E1, E2, E3>& v, T s) {
	return v() / s;
}