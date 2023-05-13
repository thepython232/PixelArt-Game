#pragma once

#include "Types.h"
#include "Std.h"
#include "State.h"
#include "Math.h"

#define LOG(_f, ...) global.log->Log(_f, util::Logger::General, util::Logger::Debug, __PRETTY_FILE__, __LINE__, __func__, __VA_ARGS__)
#define WARN(_f, ...) global.log->Log(_f, util::Logger::General, util::Logger::Warn, __PRETTY_FILE__, __LINE__, __func__, __VA_ARGS__)
#define LOGNG(_t, _f, ...) global.log->Log(_f, _t, util::Logger::Debug, __PRETTY_FILE__, __LINE__, __func__, __VA_ARGS__)
#define WARNNG(_t, _f, ...) global.log->Log(_f, _t, util::Logger::Warn, __PRETTY_FILE__, __LINE__, __func__, __VA_ARGS__)
#define LOGNF(_f, ...) global.log->LogNoFile(_f, util::Logger::General, util::Logger::Debug, __VA_ARGS__)
#define LOGNFNG(_t, _f, ...) global.log->LogNoFile(_f, _t, util::Logger::Debug, __VA_ARGS__)
#undef ERROR
#define ERROR(_e, _t, _f, ...) do { \
	global.log->Log(_f, _t, util::Logger::Error, __PRETTY_FILE__, __LINE__, __func__, __VA_ARGS__); \
	global.log->Abort(_e); \
} while (0)

namespace util {

	struct Logger {
		enum LogType {
			General,
			Vulkan,
			GLFW,
			GFX,
			Physics,
			ECS,
			TCS,
			ICS
		};

		enum Severity {
			Debug,
			Warn,
			Error
		};

		Logger(std::ostream& out, std::ostream& err)
			: general(out), error(err) { }

		static std::string ToString(const char* s) {
			return std::string(s);
		}

		template<typename T>
		static std::string ToString(const T* t) {
			std::stringstream ss;
			ss << t;
			return ss.str();
		}

		static std::string ToString(const std::string& s) {
			return s;
		}

		template<typename T>
			requires requires (T t) {
				std::to_string(t);
			}
		static std::string ToString(const T& t) {
			return std::to_string(t);
		}

		template<math::Numeric T, usize L>
		static std::string ToString(const math::vec<L, T>& v) {
			std::string s;
			if constexpr (std::same_as<T, f64>) {
				s += "d";
			}
			else if constexpr (std::same_as<T, b8>) {
				s += "b";
			}
			else if constexpr (math::Integral<T>) {
				if constexpr (math::Signed<T>) {
					s += "i";
				}
				else {
					s += "u";
				}
			}

			s += "vec";
			s += std::to_string(L);
			s += "(";
			for (usize i = 0; i < L; i++) {
				s += std::to_string(v[i]);
				if (i < L - 1) s += ", ";
			}
			s += ")";
			return s;
		}

		template<typename... Args>
		void Log(const std::string& format, LogType type, Severity severity, const char* file, int line, const char* func, const Args&... args) {
			std::ostream& stream = (type == Debug || type == Warn) ? general : error;

			switch (severity)
			{
			case util::Logger::Debug:
				stream << "[DEBUG]";
				break;
			case util::Logger::Warn:
				stream << "[WARN]";
				break;
			case util::Logger::Error:
				stream << "[ERROR]";
				break;
			}

			switch (type)
			{
			case util::Logger::General:
				stream << "[GENERAL]";
				break;
			case util::Logger::Vulkan:
				stream << "[VULKAN]";
				break;
			case util::Logger::GLFW:
				stream << "[GLFW]";
				break;
			case util::Logger::GFX:
				stream << "[GFX]";
				break;
			case util::Logger::Physics:
				stream << "[PHSX]";
				break;
			case util::Logger::ECS:
				stream << "[ECS]";
				break;
			case util::Logger::TCS:
				stream << "[TCS]";
				break;
			case util::Logger::ICS:
				stream << "[ICS]";
				break;
			}

			stream << "[" << func << " (" << file << ":" << line << ")] ";

			auto iter = format.begin();
			([&] {
				while (*iter != '$' || (iter != format.end() - 1 && *(iter + 1) == '$')) {
					if (*iter == '$') iter++;

					stream << *iter;

					iter++;
				}

				stream << ToString(args);

				iter++;

				} (), ...);

			while (iter != format.end()) {
				stream << *iter;
				iter++;
			}

			stream << std::endl;
		}

		void Log(const std::string& format, LogType type, Severity severity, const char* file, int line, const char* func) {
			std::ostream& stream = (type == Debug || type == Warn) ? general : error;

			switch (severity)
			{
			case util::Logger::Debug:
				stream << "[DEBUG]";
				break;
			case util::Logger::Warn:
				stream << "[WARN]";
				break;
			case util::Logger::Error:
				stream << "[ERROR]";
				break;
			}

			switch (type)
			{
			case util::Logger::General:
				stream << "[GENERAL]";
				break;
			case util::Logger::Vulkan:
				stream << "[VULKAN]";
				break;
			case util::Logger::GLFW:
				stream << "[GLFW]";
				break;
			case util::Logger::GFX:
				stream << "[GFX]";
				break;
			case util::Logger::Physics:
				stream << "[PHSX]";
				break;
			case util::Logger::ECS:
				stream << "[ECS]";
				break;
			case util::Logger::TCS:
				stream << "[TCS]";
				break;
			case util::Logger::ICS:
				stream << "[ICS]";
				break;
			}

			stream << "[" << func << " (" << file << ":" << line << ")] ";

			stream << format;

			stream << std::endl;
		}

		template<typename... Args>
		void LogNoFile(const std::string& format, LogType type, Severity severity, const Args&... args) {
			std::ostream& stream = (type == Debug || type == Warn) ? general : error;

			switch (severity)
			{
			case util::Logger::Debug:
				stream << "[DEBUG]";
				break;
			case util::Logger::Warn:
				stream << "[WARN]";
				break;
			case util::Logger::Error:
				stream << "[ERROR]";
				break;
			}

			switch (type)
			{
			case util::Logger::General:
				stream << "[GENERAL] ";
				break;
			case util::Logger::Vulkan:
				stream << "[VULKAN] ";
				break;
			case util::Logger::GLFW:
				stream << "[GLFW] ";
				break;
			case util::Logger::GFX:
				stream << "[GFX] ";
				break;
			case util::Logger::Physics:
				stream << "[PHSX] ";
				break;
			case util::Logger::ECS:
				stream << "[ECS] ";
				break;
			case util::Logger::TCS:
				stream << "[TCS] ";
				break;
			case util::Logger::ICS:
				stream << "[ICS] ";
				break;
			}

			auto iter = format.begin();
			([&] {
				while (*iter != '$' || (iter != format.end() - 1 && *(iter + 1) == '$')) {
					if (*iter == '$') iter++;

					stream << *iter;

					iter++;
				}

				stream << ToString(args);

				iter++;

				} (), ...);

			while (iter != format.end()) {
				stream << *iter;
				iter++;
			}

			stream << std::endl;
		}

		void LogNoFile(const std::string& format, LogType type, Severity severity) {
			std::ostream& stream = (type == Debug || type == Warn) ? general : error;

			switch (severity)
			{
			case util::Logger::Debug:
				stream << "[DEBUG]";
				break;
			case util::Logger::Warn:
				stream << "[WARN]";
				break;
			case util::Logger::Error:
				stream << "[ERROR]";
				break;
			}

			switch (type)
			{
			case util::Logger::General:
				stream << "[GENERAL] ";
				break;
			case util::Logger::Vulkan:
				stream << "[VULKAN] ";
				break;
			case util::Logger::GLFW:
				stream << "[GLFW] ";
				break;
			case util::Logger::GFX:
				stream << "[GFX] ";
				break;
			case util::Logger::Physics:
				stream << "[PHSX] ";
				break;
			case util::Logger::ECS:
				stream << "[ECS] ";
				break;
			case util::Logger::TCS:
				stream << "[TCS] ";
				break;
			case util::Logger::ICS:
				stream << "[ICS] ";
				break;
			}

			stream << format;

			stream << std::endl;
		}

		void Abort(i32 error = EXIT_FAILURE) {
#ifdef GAME_IS_DEBUG
			__debugbreak();
#endif // GAME_IS_DEBUG
			exit(error);
		}

		std::ostream& general;
		std::ostream& error;
	};
}