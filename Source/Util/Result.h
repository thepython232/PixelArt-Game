#pragma once

#include "Types.h"

namespace types {
	template<typename T>
	struct Ok {
		Ok(const T& val) : val(val) { }
		Ok(T&& val) : val(std::move(val)) { }

		T val;
	};

	template<>
	struct Ok<void> { };

	template<typename U>
	struct Err {
		Err(const U& val) : val(val) { }
		Err(U&& val) : val(std::move(val)) { }

		U val;
	};

	template<typename T, typename U>
	struct Storage {
		static constexpr usize Size = (sizeof(T) > sizeof(U) ? sizeof(T) : sizeof(U));
		static constexpr usize Align = (sizeof(T) > sizeof(U) ? alignof(T) : alignof(U));

		using AlignType = std::aligned_storage_t<Size, Align>;

		Storage(T&& ok) {
			new (&storage) T(std::move(ok));
		}

		Storage(U&& err) {
			new (&storage) U(std::move(err));
		}

		template<typename V>
			requires std::same_as<T, V>
		T& Get() {
			return *reinterpret_cast<T*>(&storage);
		}

		template<typename V>
			requires std::same_as<U, V>
		U& Get() {
			return *reinterpret_cast<U*>(&storage);
		}

	private:
		AlignType storage;
	};

	template<typename T>
	struct Storage<T, T> {
		Storage(T&& t) : t(std::move(t)) { }

		template<typename T>
		T& Get() {
			return t;
		}

	private:
		T t;
	};

	template<typename U>
	struct Storage<void, U> {
		Storage(U&& u) : u(std::move(u)) { }

		template<typename>
		U& Get() {
			return u;
		}

	private:
		U u;
	};
}

template<typename T, typename U>
struct Result {
	Result(T&& t) : error(false), storage(std::move(t)) { }

	template<typename V>
		requires std::convertible_to<V, T>
	Result(types::Ok<V>&& ok) : storage(std::move(ok.val)), error(false) { }

	template<typename V>
		requires std::convertible_to<V, U>
	Result(types::Err<V>&& err) : storage(std::move(err.val)), error(true) { }

	Result(const Result& other) = delete;
	Result& operator=(const Result& other) = delete;
	Result(Result&& other) = default;
	Result& operator=(Result&& other) = default;

	inline explicit operator bool() const {
		return !error;
	}

	T Expect(const std::string& failMsg) {
		if (!error) {
			return std::move(storage.template Get<T>());
		}
		else {
			std::cerr << failMsg << std::endl;
			abort();
		}
	}

	T Unwrap() {
		if (error) {
			std::cerr << "Can't unwrap error Result value!" << std::endl;
			abort();
		}
		else {
			return std::move(storage.template Get<T>());
		}
	}

	T UnwrapOr(const T& val) {
		if (error) {
			return val;
		}
		else {
			return std::move(storage.template Get<T>());
		}
	}

	U UnwrapErr() {
		if (error) {
			return std::move(storage.template Get<U>());
		}

		std::cerr << "Can't unwrap ok Result value!" << std::endl;
		abort();
	}

	inline bool IsOk() const {
		return !error;
	}

	inline bool IsErr() const {
		return error;
	}

private:
	types::Storage<T, U> storage;

	bool error;
};

template<typename U>
struct Result<void, U> {
	Result() : error(false) { }

	Result(types::Ok<void>&& ok)
		: error(false) { }

	template<typename V>
		requires std::convertible_to<V, U>
	Result(types::Err<V>&& err)
		: storage(std::move(err.val)), error(true) { }

	Result(const Result& other) = delete;
	Result& operator=(const Result& other) = delete;
	Result(Result&& other) = default;
	Result& operator=(Result&& other) = default;

	inline explicit operator bool() const {
		return !error;
	}

	U UnwrapError() {
		if (error) {
			return std::move(storage);
		}

		std::cerr << "Can't unwrap ok Result value!" << std::endl;
		abort();
	}

	inline bool IsOk() const {
		return !error;
	}

	inline bool IsErr() const {
		return error;
	}

private:
	U storage;

	bool error;
};

template<typename T, typename CleanT = typename std::decay<T>::type>
types::Ok<CleanT> Ok(T&& t) {
	return types::Ok<CleanT>(std::forward<T>(t));
}

inline types::Ok<void> Ok() {
	return types::Ok<void>();
}

template<typename U, typename CleanU = typename std::decay<U>::type>
types::Err<CleanU> Err(U&& u) {
	return types::Err<CleanU>(std::forward<U>(u));
}

template<typename T, typename U>
inline bool operator==(const Result<T, U>& a, const Result<T, U>& b) {
	if (a.IsOk() && b.IsOk()) {
		return a.Unwrap() == b.Unwrap();
	}
	else if (a.IsErr() && b.IsErr()) {
		return a.UnwrapErr() == b.UnwrapErr();
	}
	else {
		return false;
	}
}

template<typename U>
inline bool operator==(const Result<void, U>& a, const Result<void, U>& b) {
	if (a.IsOk() && b.IsOk()) {
		return true;
	}
	else if (a.IsErr() && b.IsErr()) {
		return a.Unwrap() == b.Unwrap();
	}
	else {
		return false;
	}
}

template<typename T, typename U>
inline bool operator==(const Result<T, U>& res, const types::Ok<T>& ok) {
	return res.IsOk();
}

template<typename T, typename U>
inline bool operator==(const Result<T, U>& res, const types::Err<U>& err) {
	return res.IsErr();
}