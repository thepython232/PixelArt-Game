#pragma once

#include "Util\Types.h"
#include "Util\Std.h"
#include "Util\Math.h"
#include "GLFW\glfw3.h"

namespace platform {

	struct Button {
		enum class State {
			NONE,
			PRESSED,
			RELEASED,
			HELD
		};

		Button() = default;

		inline b8 Down() const { return down; }
		inline State CurrentState() const {
			if (down) {
				if (last) {
					return State::HELD;
				}
				else {
					return State::PRESSED;
				}
			}
			else {
				if (last) {
					return State::RELEASED;
				}
				else {
					return State::NONE;
				}
			}
		}

		inline State CurrentStateTick() const {
			if (down) {
				if (lastTick) {
					return State::HELD;
				}
				else {
					return State::PRESSED;
				}
			}
			else {
				if (lastTick) {
					return State::RELEASED;
				}
				else {
					return State::NONE;
				}
			}
		}

		inline explicit operator bool() const { return down; }

		inline b8 Pressed() const { return down && !last; }
		inline b8 PressedTick() const { return down && !lastTick; }

		inline b8 Released() const { return !down && last; }
		inline b8 ReleasedTick() const { return !down && lastTick; }

		inline b8 Held() const { return down && last; }
		inline b8 HeldTick() const { return down && lastTick; }

		inline b8 Repeating() const { return repeat; }

	private:
		b8 down : 1, last : 1, lastTick : 1, repeat : 1;

		friend class Keyboard;
		friend class Mouse;
	};

	constexpr u32 MAX_KEYS = GLFW_KEY_LAST + 1;

	class Keyboard {
	public:
		Keyboard();

		inline const Button& operator[](usize button) const{ return keys[button]; }
		inline const Button& operator[](const std::string& name) const{ return keys[ButtonCode(name)]; }

		inline const Button& GetButton(usize button) const { return keys[button]; }
		inline const Button& GetButton(const std::string& name) const { return keys[ButtonCode(name)]; }

		static int ButtonCode(const std::string& name);
		static std::string ButtonName(int code);

		void KeyPressed(int key, int scancode, int action, int mods);
		void Prepare();
		void PrepareTick();

	private:
		std::array<Button, MAX_KEYS> keys;
	};

	constexpr u32 MAX_MOUSE_BUTTONS = GLFW_MOUSE_BUTTON_LAST + 1;

	class Mouse {
	public:
		Mouse();

		inline const Button& operator[](usize button) const { return buttons[button]; }
		inline const Button& operator[](const std::string& name) const { return buttons[ButtonCode(name)]; }

		inline const Button& GetButton(usize button) const { return buttons[button]; }
		inline const Button& GetButton(const std::string& name) const { return buttons[ButtonCode(name)]; }

		inline vec2 Pos() const { return pos; }
		inline vec2 Delta() const { return pos - lastPos; }
		inline vec2 DeltaTick() const { return pos - lastPosTick; }

		inline vec2 Scroll() const { return scroll; }
		inline vec2 ScrollDelta() const { return scroll - lastScroll; }
		inline vec2 ScrollDeltaTick() const { return scroll - lastScrollTick; }

		inline bool Active() const { return active; }

		static int ButtonCode(const std::string& name);
		static std::string ButtonName(int code);

		void Prepare();
		void PrepareTick();

		void MouseMoved(const vec2& pos);
		void Scrolled(const vec2& offset);
		void ButtonPressed(int button, int action, int mods);
		void Enter(int entered);

	private:
		std::array<Button, MAX_MOUSE_BUTTONS> buttons;
		vec2 pos, lastPos, lastPosTick;
		vec2 scroll, lastScroll, lastScrollTick;
		bool active;

		friend class Window;
	};
}