#include "Time.h"
#include "Math.h"

namespace util {

	Time::Section::Section(Time* time, bool isTick)
		: time(time), isTick(isTick) {
		memset(static_cast<void*>(times.data()), 0, times.size() * sizeof(f64));
	}

	void Time::Section::Begin() {
		start = time->CurrentTime();
	}

	void Time::Section::End() {
		f64 elapsedTime = time->CurrentTime() - start;
		usize index = (isTick ? time->TickCount() : time->FrameCount()) % SECTION_LENGTH;
		sectionTime -= times[index];
		times[index] = elapsedTime;
		sectionTime += elapsedTime;
		count++;

		if (isTick) {
			time->lastTickTime = time->CurrentTime();
		}
	}

	f64 Time::Section::Avg() const {
		return sectionTime / math::Min(count, SECTION_LENGTH);
	}

	Time::Time(CurrentTimeFn timeFn)
		: timeFn(timeFn),
		frame(this),
		tick(this, true),
		update(this),
		prepare(this),
		render(this) {
		lastSecond = CurrentTime();
	}

	void Time::Update() {
		f64 timeSinceLastTick = CurrentTime() - lastUpdate + lastTickError;
		f64 tickTime = math::Clamp(timeSinceLastTick, 0.0, MAX_TICK_TIME);
		numTicks = math::Floor(tickTime / DELTA_TIME);
		lastTickError = math::Mod(tickTime, DELTA_TIME);
		lastUpdate = CurrentTime();

		numSecondTicks += numTicks;
		if (CurrentTime() - lastSecond >= 1000.0) {
			lastSecond = CurrentTime();
			oldTps = tps;
			tps = static_cast<f64>(numSecondTicks);
			numSecondTicks = 0;
		}
	}

	f64 Time::AvgFPS() const {
		return 1000.0 / frame.Avg();
	}
}