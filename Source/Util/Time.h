#pragma once

#include "Types.h"
#include "Std.h"

namespace util {

	class Time {
	public:
		static constexpr usize TPS = 100;
		static constexpr usize SECTION_LENGTH = 60;
		static constexpr f64 DELTA_TIME = 1000.0 / static_cast<f64>(TPS); //In ms
		static constexpr f64 MAX_TICK_TIME = 40.0; //In ms

		using CurrentTimeFn = std::function<f64(void)>; //In ms

		struct Section {
			Section(Time* time, bool isTick = false);

			void Begin();
			void End();

			f64 Avg() const;
			inline u64 Count() const { return count; }

		private:
			f64 start = 0.0;
			std::array<f64, SECTION_LENGTH> times;
			u64 count = 0;
			f64 sectionTime = 0.0;
			bool isTick;
			Time* time;

			friend Time;
		} frame, tick, update, prepare, render;

		Time(CurrentTimeFn timeFn);

		inline operator f64() const {
			return timeFn();
		}

		inline f64 CurrentTime() const {
			return timeFn();
		}

		inline u64 FrameCount() const {
			return frame.Count();
		}

		inline u64 TickCount() const {
			return tick.Count();
		}

		void Update();
		u32 NumTicks() { return numTicks; }

		f64 AvgFPS() const;
		inline f64 AvgTPS() const { return (tps + oldTps) * 0.5; }

	private:
		CurrentTimeFn timeFn;
		f64 lastTickError, lastTickTime;
		f64 lastUpdate;
		u32 numTicks;
		u32 numSecondTicks;
		f64 lastSecond, tps = 0.0, oldTps = 0.0;
	};
}