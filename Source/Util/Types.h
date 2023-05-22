#pragma once

#include <cstdint>

#if (INTPTR_MAX == INT64_MAX)
#define GAME_IS_x64
#else
#define GAME_IS_x86
#endif

#ifdef NDEBUG
#define GAME_IS_RELEASE
#else
#define GAME_IS_DEBUG
#endif

#define UNUSED [[maybe_unused]]

#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
#define GAME_IS_WINDOWS
#elif defined(__unix__)
#define GAME_IS_LINUX
#elif defined(__linux__)
#define GAME_IS_LINUX
#elif defined(__APPLE__)
#include <TargetConditionals.h>
#if TARGET_IHPONE_SIMULATOR==1
#define GAME_IS_IOS
#elif TARGET_OS_IPHONE==1
#define GAME_IS_IOS
#elif TARGET_OS_MAC==1
#define GAME_IS_OSX
#endif
#elif defined(__ANDROID__)
#define GAME_IS_ANDROID
#endif

typedef int                  i32;
typedef unsigned int         u32;
typedef short                i16;
typedef unsigned short       u16;
typedef signed char           i8;
typedef unsigned char         u8;
typedef long long            i64; //TODO: fix this (or get a better compiler)
typedef unsigned long long   u64;
typedef float                f32;
typedef double               f64;
#ifdef GAME_HAS_FLOAT128
typedef long double         f128;
#endif
typedef bool                  b8;
typedef unsigned int         b32;
#ifdef GAME_HAS_INT128
typedef long long           i128;
typedef unsigned long long  u128;
#endif
#ifdef GAME_IS_x64
typedef long long          isize;
typedef unsigned long long usize;
#else
typedef int                isize;
typedef unsigned int       usize;
#endif;

#include <tuple>

namespace types {
	struct Empty { };

	template<typename T>
	struct FunctionTraits;

	template<typename R, typename... Args>
	struct FunctionTraits<R(Args...)> {
		using ReturnType = R;
		using Arguments = std::tuple<Args...>;

		template<usize I>
		using Argument = std::tuple_element_t<I, Arguments>;
	};

	template<typename T>
	struct MemberFunctionTraits;

	template<typename R, typename I, typename... Args>
	struct MemberFunctionTraits<R(I::*)(Args...)> {
		using ReturnType = R;
		using InstanceType = I;
		using Arguments = std::tuple<Args...>;

		template<usize I>
		using Argument = std::tuple_element_t<I, Arguments>;
	};

	template<typename T>
	struct IntToFloat {
		using Type = f32;
	};

	template<>
	struct IntToFloat<f64> {
		using Type = f64;
	};

	template<typename T>
	using ToFloatType = IntToFloat<T>::Type;

	template<typename T, typename U>
	struct IsSame {
		static constexpr bool Value = false;
	};

	template<typename T>
	struct IsSame<T, T> {
		static constexpr bool Value = true;
	};

	template<typename T, typename U>
	concept SameAs = IsSame<T, U>::Value;

	template<typename T, typename... Ts>
	struct AreSame {
		static constexpr bool Value = (IsSame<T, Ts>::Value && ...);
	};

	template<typename T>
	struct AreSame<T> {
		static constexpr bool Value = true;
	};

	template<typename T, typename... Ts>
	concept AllSame = AreSame<T, Ts...>::Value;

	template<typename T, typename... Ts>
	struct IsAny {
		static constexpr bool Value = (IsSame<T, Ts>::Value || ...);
	};

	template<typename T>
	struct IsAny<T> {
		static constexpr bool Value = false;
	};

	template<typename T, typename... Ts>
	concept AnySame = IsAny<T, Ts...>::Value;
}

#include <array>
template<typename... Ts>
auto make_array(Ts&&... ts) ->
	std::array<
		typename std::decay<
			typename std::common_type<Ts...>::type
		>::type,
	sizeof...(Ts)>
{
	return std::array<
			typename std::decay<
				typename std::common_type<Ts...>::type
			>::type,
		sizeof...(Ts)>{ std::forward<Ts>(ts)... };
}

#define ASSERT(_c, _m) if (!(_c)) { std::cerr << "Assertion Failed (" << _m << ")!" << std::endl; __debugbreak(); abort(); } else

#undef max
#undef min
#undef near
#undef far
#undef INFINITY