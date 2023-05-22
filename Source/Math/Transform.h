#pragma once

#include "Common.h"
#include "TypeMat.h"
#include "TypeVec.h"

//All transforms apply on top of the given matrix
//Ex: Scale(m, {2.f}) -> scale({2.f}) * m;
namespace math {

	template<Numeric T = f32>
	constexpr inline mat<4, 4, T> Scale(const mat<4, 4, T>& m, const vec<3, T>& s) {
		return mat<4, 4, T>(s.x, s.y, s.z, 1) * m;
	}

	template<Numeric T = f32>
	constexpr inline mat<3, 3, T> Scale(const mat<3, 3, T>& m, const vec<3, T>& s) {
		return mat<3, 3, T>(s.x, s.y, s.z) * m;
	}

	template<Numeric T = f32>
	constexpr inline mat<3, 3, T> Scale(const mat<3, 3, T>& m, const vec<2, T>& s) {
		return mat<3, 3, T>(s.x, s.y, 1) * m;
	}

	template<Numeric T = f32>
	constexpr inline mat<2, 2, T> Scale(const mat<2, 2, T>& m, const vec<2, T>& s) {
		return mat<2, 2, T>(s.x, s.y) * m;
	}

	//Here: https://en.wikipedia.org/wiki/Rotation_matrix#Rotation_matrix_from_axis_and_angle
	template<Numeric T = f32>
	inline mat<4, 4, T> Rotate(const mat<4, 4, T>& m, const vec<3, T>& axis, T angle) {
		T cosTheta = Cos(angle);
		T oneMinusCosTheta = 1 - cosTheta;
		T sinTheta = Sin(angle);

		return mat<4, 4, T>(
			cosTheta + axis.x * axis.x * oneMinusCosTheta,
			axis.y * axis.x * oneMinusCosTheta + axis.z * sinTheta,
			axis.z * axis.x * oneMinusCosTheta - axis.y * sinTheta,
			0,

			axis.x * axis.y * oneMinusCosTheta - axis.z * sinTheta,
			cosTheta + axis.y * axis.y * oneMinusCosTheta,
			axis.z * axis.y * oneMinusCosTheta + axis.x * sinTheta,
			0,

			axis.x * axis.z * oneMinusCosTheta + axis.y * sinTheta,
			axis.y * axis.z * oneMinusCosTheta - axis.x * sinTheta,
			cosTheta + axis.z * axis.z * oneMinusCosTheta,
			0,

			0, 0, 0, 1
			) * m;
	}

	template<Numeric T = f32>
	inline mat<3, 3, T> Rotate(const mat<3, 3, T>& m, const vec<3, T>& axis, T angle) {
		T cosTheta = Cos(angle);
		T oneMinusCosTheta = 1 - cosTheta;
		T sinTheta = Sin(angle);

		return mat<3, 3, T>(
			cosTheta + axis.x * axis.x * oneMinusCosTheta,
			axis.y * axis.x * oneMinusCosTheta + axis.z * sinTheta,
			axis.z * axis.x * oneMinusCosTheta - axis.y * sinTheta,

			axis.x * axis.y * oneMinusCosTheta - axis.z * sinTheta,
			cosTheta + axis.y * axis.y * oneMinusCosTheta,
			axis.z * axis.y * oneMinusCosTheta + axis.x * sinTheta,

			axis.x * axis.z * oneMinusCosTheta + axis.y * sinTheta,
			axis.y * axis.z * oneMinusCosTheta - axis.x * sinTheta,
			cosTheta + axis.z * axis.z * oneMinusCosTheta
			) * m;
	}

	template<Numeric T = f32>
	inline mat<3, 3, T> Rotate(const mat<3, 3, T>& m, T angle) {
		T cosTheta = Cos(angle);
		T sinTheta = Sin(angle);

		return mat<3, 3, T>(
			cosTheta, sinTheta, 0,
			-sinTheta, cosTheta, 0,
			0, 0, 1
			) * m;
	}

	template<Numeric T = f32>
	inline mat<2, 2, T> Rotate(const mat<2, 2, T>& m, T angle) {
		T cosTheta = Cos(angle);
		T sinTheta = Sin(angle);

		return mat<2, 2, T>(
			cosTheta, sinTheta,
			-sinTheta, cosTheta
			) * m;
	}

	template<Numeric T = f32>
	constexpr inline mat<4, 4, T> Translate(const mat<4, 4, T>& m, const vec<3, T>& v) {
		mat<4, 4, T> ret = mat<4, 4, T>::Identity;
		ret[3] = vec<4, T>(v, 1);

		return ret;
	}

	template<Numeric T = f32>
	constexpr inline mat<3, 3, T> Translate(const mat<3, 3, T>& m, const vec<2, T>& v) {
		mat<3, 3, T> ret = mat<3, 3, T>::Identity;
		ret[2] = vec<3, T>(v, 1);

		return ret;
	}

	template<Numeric T = f32>
	constexpr inline mat<4, 4, T> Ortho(T left, T right, T top, T bottom, T near, T far) {
		return mat<4, 4, T>(
			2 / (right - left), 0, 0, 0,
			0, 2 / (bottom - top), 0, 0,
			0, 0, 2 / (far - near), 0,
			-(left + right) / (right - left),
			-(top + bottom) / (bottom - top),
			-near / (far - near),
			1
			);
	}

	template<Numeric T = f32>
	constexpr inline mat<4, 4, T> Ortho(T width, T height, T near, T far) {
		return mat<4, 4, T>(
			2 / width, 0, 0, 0,
			0, -2 / height, 0, 0,
			0, 0, 2 / (far - near), 0,
			0, 0, -near / (far - near), 1
			);
	}

	template<Numeric T = f32>
	constexpr inline mat<4, 4, T> Persp(T fov, T aspect, T near, T far) {
		T height = Tan(fov / 2);
		T width = height * aspect;
		return mat<4, 4, T>(
			1 / width, 0, 0, 0,
			0, -1 / height, 0, 0,
			0, 0, far / (far - near), 1,
			0, 0, -far * near / (far - near), 0
			);
	}

	template<Numeric T = f32>
	constexpr inline mat<4, 4, T> LookAt(const vec<3, T>& eye, const vec<3, T>& target, const vec<3, T>& up = vec<3, T>::Up) {
		vec<3, T> forward = Normalize(target - eye);
		vec<3, T> right = Normalize(Cross(up, forward));
		vec<3, T> localUp = Normalize(Cross(forward, right));

		T translationX = -Dot(eye, right);
		T translationY = -Dot(eye, localUp);
		T translationZ = -Dot(eye, forward);

		return mat<4, 4, T>(
			right.x, localUp.x, forward.x, 0,
			right.y, localUp.y, forward.y, 0,
			right.z, localUp.z, forward.z, 0,
			translationX, translationY, translationZ, 1
			);
	}
}