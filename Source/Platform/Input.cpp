#include "Input.h"

namespace platform {

	static const std::unordered_map<int, std::string> keyNames = {
	{ GLFW_KEY_SPACE,                  "Space" },
	{ GLFW_KEY_APOSTROPHE,                 "'" },
	{ GLFW_KEY_COMMA,                      "," },
	{ GLFW_KEY_MINUS,                      "-" },
	{ GLFW_KEY_PERIOD,                     "." },
	{ GLFW_KEY_SLASH,                      "/" },
	{ GLFW_KEY_0,                          "0" },
	{ GLFW_KEY_1,                          "1" },
	{ GLFW_KEY_2,                          "2" },
	{ GLFW_KEY_3,                          "3" },
	{ GLFW_KEY_4,                          "4" },
	{ GLFW_KEY_5,                          "5" },
	{ GLFW_KEY_6,                          "6" },
	{ GLFW_KEY_7,                          "7" },
	{ GLFW_KEY_8,                          "8" },
	{ GLFW_KEY_9,                          "9" },
	{ GLFW_KEY_SEMICOLON,                  ";" },
	{ GLFW_KEY_EQUAL,                      "=" },
	{ GLFW_KEY_A,                          "A" },
	{ GLFW_KEY_B,                          "B" },
	{ GLFW_KEY_C,                          "C" },
	{ GLFW_KEY_D,                          "D" },
	{ GLFW_KEY_E,                          "E" },
	{ GLFW_KEY_F,                          "F" },
	{ GLFW_KEY_G,                          "G" },
	{ GLFW_KEY_H,                          "H" },
	{ GLFW_KEY_I,                          "I" },
	{ GLFW_KEY_J,                          "J" },
	{ GLFW_KEY_K,                          "K" },
	{ GLFW_KEY_L,                          "L" },
	{ GLFW_KEY_M,                          "M" },
	{ GLFW_KEY_N,                          "N" },
	{ GLFW_KEY_O,                          "O" },
	{ GLFW_KEY_P,                          "P" },
	{ GLFW_KEY_Q,                          "Q" },
	{ GLFW_KEY_R,                          "R" },
	{ GLFW_KEY_S,                          "S" },
	{ GLFW_KEY_T,                          "T" },
	{ GLFW_KEY_U,                          "U" },
	{ GLFW_KEY_V,                          "V" },
	{ GLFW_KEY_W,                          "W" },
	{ GLFW_KEY_X,                          "X" },
	{ GLFW_KEY_Y,                          "Y" },
	{ GLFW_KEY_Z,                          "Z" },
	{ GLFW_KEY_LEFT_BRACKET,               "[" },
	{ GLFW_KEY_BACKSLASH,                 "\\" },
	{ GLFW_KEY_RIGHT_BRACKET ,             "]" },
	{ GLFW_KEY_GRAVE_ACCENT,               "`" },
	{ GLFW_KEY_WORLD_1,              "World 1" },
	{ GLFW_KEY_WORLD_2,              "World 2" },
	{ GLFW_KEY_ESCAPE,                "Escape" },
	{ GLFW_KEY_ENTER,                  "Enter" },
	{ GLFW_KEY_TAB,                      "Tab" },
	{ GLFW_KEY_BACKSLASH,          "Backspace" },
	{ GLFW_KEY_INSERT,                "Insert" },
	{ GLFW_KEY_DELETE,                "Delete" },
	{ GLFW_KEY_RIGHT,                  "Right" },
	{ GLFW_KEY_LEFT,                    "Left" },
	{ GLFW_KEY_DOWN,                    "Down" },
	{ GLFW_KEY_UP,                        "Up" },
	{ GLFW_KEY_PAGE_UP,              "Page Up" },
	{ GLFW_KEY_PAGE_DOWN,          "Page Down" },
	{ GLFW_KEY_HOME,                    "Home" },
	{ GLFW_KEY_END,                      "End" },
	{ GLFW_KEY_CAPS_LOCK,          "Caps Lock" },
	{ GLFW_KEY_SCROLL_LOCK,      "Scroll Lock" },
	{ GLFW_KEY_NUM_LOCK,            "Num Lock" },
	{ GLFW_KEY_PRINT_SCREEN,    "Print Screen" },
	{ GLFW_KEY_PAUSE,                  "Pause" },
	{ GLFW_KEY_F1,                        "F1" },
	{ GLFW_KEY_F2,                        "F2" },
	{ GLFW_KEY_F3,                        "F3" },
	{ GLFW_KEY_F4,                        "F4" },
	{ GLFW_KEY_F5,                        "F5" },
	{ GLFW_KEY_F6,                        "F6" },
	{ GLFW_KEY_F7,                        "F7" },
	{ GLFW_KEY_F8,                        "F8" },
	{ GLFW_KEY_F9,                        "F9" },
	{ GLFW_KEY_F10,                      "F10" },
	{ GLFW_KEY_F11,                      "F11" },
	{ GLFW_KEY_F12,                      "F12" },
	{ GLFW_KEY_F12,                      "F12" },
	{ GLFW_KEY_F13,                      "F13" },
	{ GLFW_KEY_F14,                      "F14" },
	{ GLFW_KEY_F15,                      "F15" },
	{ GLFW_KEY_F16,                      "F16" },
	{ GLFW_KEY_F17,                      "F17" },
	{ GLFW_KEY_F18,                      "F18" },
	{ GLFW_KEY_F19,                      "F19" },
	{ GLFW_KEY_F20,                      "F20" },
	{ GLFW_KEY_F21,                      "F21" },
	{ GLFW_KEY_F22,                      "F22" },
	{ GLFW_KEY_F23,                      "F23" },
	{ GLFW_KEY_F24,                      "F24" },
	{ GLFW_KEY_F25,                      "F25" },
	{ GLFW_KEY_KP_0,                "KeyPad 0" },
	{ GLFW_KEY_KP_1,                "KeyPad 1" },
	{ GLFW_KEY_KP_2,                "KeyPad 2" },
	{ GLFW_KEY_KP_3,                "KeyPad 3" },
	{ GLFW_KEY_KP_4,                "KeyPad 4" },
	{ GLFW_KEY_KP_5,                "KeyPad 5" },
	{ GLFW_KEY_KP_6,                "KeyPad 6" },
	{ GLFW_KEY_KP_7,                "KeyPad 7" },
	{ GLFW_KEY_KP_8,                "KeyPad 8" },
	{ GLFW_KEY_KP_9,                "KeyPad 9" },
	{ GLFW_KEY_KP_DECIMAL,    "KeyPad Decimal" },
	{ GLFW_KEY_KP_DIVIDE,      "KeyPad Divide" },
	{ GLFW_KEY_KP_MULTIPLY,  "KeyPad Multiply" },
	{ GLFW_KEY_KP_SUBTRACT,  "KeyPad Subtract" },
	{ GLFW_KEY_KP_ADD,            "KeyPad Add" },
	{ GLFW_KEY_KP_ENTER,        "KeyPad Enter" },
	{ GLFW_KEY_KP_EQUAL,        "KeyPad Equal" },
	{ GLFW_KEY_LEFT_SHIFT,        "Left Shift" },
	{ GLFW_KEY_LEFT_CONTROL,   "Left Control" },
	{ GLFW_KEY_LEFT_ALT,            "Left Alt" },
	{ GLFW_KEY_LEFT_SUPER,        "Left Super" },
	{ GLFW_KEY_RIGHT_SHIFT,      "Right Shift" },
	{ GLFW_KEY_RIGHT_CONTROL,  "Right Control" },
	{ GLFW_KEY_RIGHT_ALT,          "Right Alt" },
	{ GLFW_KEY_RIGHT_SUPER,      "Right Super" },
	{ GLFW_KEY_MENU,                    "Menu" },
	{ GLFW_KEY_LAST,                    "Menu" }
	};
	static const std::unordered_map<std::string, int> keyCodes = {
		{ "Space",                  GLFW_KEY_SPACE },
		{ "'",                 GLFW_KEY_APOSTROPHE },
		{ ",",                      GLFW_KEY_COMMA },
		{ "-",                      GLFW_KEY_MINUS },
		{ ".",                     GLFW_KEY_PERIOD },
		{ "/",                      GLFW_KEY_SLASH },
		{ "0",                          GLFW_KEY_0 },
		{ "1",                          GLFW_KEY_1 },
		{ "2",                          GLFW_KEY_2 },
		{ "3",                          GLFW_KEY_3 },
		{ "4",                          GLFW_KEY_4 },
		{ "5",                          GLFW_KEY_5 },
		{ "6",                          GLFW_KEY_6 },
		{ "7",                          GLFW_KEY_7 },
		{ "8",                          GLFW_KEY_8 },
		{ "9",                          GLFW_KEY_9 },
		{ ";",                  GLFW_KEY_SEMICOLON },
		{ "=",                      GLFW_KEY_EQUAL },
		{ "A",                          GLFW_KEY_A },
		{ "B",                          GLFW_KEY_B },
		{ "C",                          GLFW_KEY_C },
		{ "D",                          GLFW_KEY_D },
		{ "E",                          GLFW_KEY_E },
		{ "F",                          GLFW_KEY_F },
		{ "G",                          GLFW_KEY_G },
		{ "H",                          GLFW_KEY_H },
		{ "I",                          GLFW_KEY_I },
		{ "J",                          GLFW_KEY_J },
		{ "K",                          GLFW_KEY_K },
		{ "L",                          GLFW_KEY_L },
		{ "M",                          GLFW_KEY_M },
		{ "N",                          GLFW_KEY_N },
		{ "O",                          GLFW_KEY_O },
		{ "P",                          GLFW_KEY_P },
		{ "Q",                          GLFW_KEY_Q },
		{ "R",                          GLFW_KEY_R },
		{ "S",                          GLFW_KEY_S },
		{ "T",                          GLFW_KEY_T },
		{ "U",                          GLFW_KEY_U },
		{ "V",                          GLFW_KEY_V },
		{ "W",                          GLFW_KEY_W },
		{ "X",                          GLFW_KEY_X },
		{ "Y",                          GLFW_KEY_Y },
		{ "Z",                          GLFW_KEY_Z },
		{ "[",               GLFW_KEY_LEFT_BRACKET },
		{ "\\",                 GLFW_KEY_BACKSLASH },
		{ "]",              GLFW_KEY_RIGHT_BRACKET },
		{ "`",               GLFW_KEY_GRAVE_ACCENT },
		{"World 1",               GLFW_KEY_WORLD_1 },
		{"World 2",               GLFW_KEY_WORLD_2 },
		{"Escape",                 GLFW_KEY_ESCAPE },
		{"Enter",                   GLFW_KEY_ENTER },
		{"Tab",                       GLFW_KEY_TAB },
		{"Backspace",           GLFW_KEY_BACKSLASH },
		{"Insert",                 GLFW_KEY_INSERT },
		{"Delete",                 GLFW_KEY_DELETE },
		{"Right",                   GLFW_KEY_RIGHT },
		{"Left",                     GLFW_KEY_LEFT },
		{"Down",                     GLFW_KEY_DOWN },
		{"Up",                         GLFW_KEY_UP },
		{"Page Up",               GLFW_KEY_PAGE_UP },
		{"Page Down",           GLFW_KEY_PAGE_DOWN },
		{"Home",                     GLFW_KEY_HOME },
		{"End",                       GLFW_KEY_END },
		{"Caps Lock",           GLFW_KEY_CAPS_LOCK },
		{"Scroll Lock",       GLFW_KEY_SCROLL_LOCK },
		{"Num Lock",             GLFW_KEY_NUM_LOCK },
		{"Print Screen",     GLFW_KEY_PRINT_SCREEN },
		{"Pause",                   GLFW_KEY_PAUSE },
		{"F1",                         GLFW_KEY_F1 },
		{"F2",                         GLFW_KEY_F2 },
		{"F3",                         GLFW_KEY_F3 },
		{"F4",                         GLFW_KEY_F4 },
		{"F5",                         GLFW_KEY_F5 },
		{"F6",                         GLFW_KEY_F6 },
		{"F7",                         GLFW_KEY_F7 },
		{"F8",                         GLFW_KEY_F8 },
		{"F9",                         GLFW_KEY_F9 },
		{"F10",                       GLFW_KEY_F10 },
		{"F11",                       GLFW_KEY_F11 },
		{"F12",                       GLFW_KEY_F12 },
		{"F12",                       GLFW_KEY_F12 },
		{"F13",                       GLFW_KEY_F13 },
		{"F14",                       GLFW_KEY_F14 },
		{"F15",                       GLFW_KEY_F15 },
		{"F16",                       GLFW_KEY_F16 },
		{"F17",                       GLFW_KEY_F17 },
		{"F18",                       GLFW_KEY_F18 },
		{"F19",                       GLFW_KEY_F19 },
		{"F20",                       GLFW_KEY_F20 },
		{"F21",                       GLFW_KEY_F21 },
		{"F22",                       GLFW_KEY_F22 },
		{"F23",                       GLFW_KEY_F23 },
		{"F24",                       GLFW_KEY_F24 },
		{"F25",                       GLFW_KEY_F25 },
		{"KeyPad 0",                 GLFW_KEY_KP_0 },
		{"KeyPad 1",                 GLFW_KEY_KP_1 },
		{"KeyPad 2",                 GLFW_KEY_KP_2 },
		{"KeyPad 3",                 GLFW_KEY_KP_3 },
		{"KeyPad 4",                 GLFW_KEY_KP_4 },
		{"KeyPad 5",                 GLFW_KEY_KP_5 },
		{"KeyPad 6",                 GLFW_KEY_KP_6 },
		{"KeyPad 7",                 GLFW_KEY_KP_7 },
		{"KeyPad 8",                 GLFW_KEY_KP_8 },
		{"KeyPad 9",                 GLFW_KEY_KP_9 },
		{"KeyPad Decimal",     GLFW_KEY_KP_DECIMAL },
		{"KeyPad Divide",       GLFW_KEY_KP_DIVIDE },
		{"KeyPad Multiply",   GLFW_KEY_KP_MULTIPLY },
		{"KeyPad Subtract",   GLFW_KEY_KP_SUBTRACT },
		{"KeyPad Add",             GLFW_KEY_KP_ADD },
		{"KeyPad Enter",         GLFW_KEY_KP_ENTER },
		{"KeyPad Equal",         GLFW_KEY_KP_EQUAL },
		{"Left Shift",         GLFW_KEY_LEFT_SHIFT },
		{"Left Control",    GLFW_KEY_RIGHT_CONTROL },
		{"Left Alt",             GLFW_KEY_LEFT_ALT },
		{"Left Super",         GLFW_KEY_LEFT_SUPER },
		{"Right Shift",       GLFW_KEY_RIGHT_SHIFT },
		{"Right Control",   GLFW_KEY_RIGHT_CONTROL },
		{"Right Alt",           GLFW_KEY_RIGHT_ALT },
		{"Right Super",       GLFW_KEY_RIGHT_SUPER },
		{"Menu",                     GLFW_KEY_MENU }
	};

	static const std::unordered_map<int, std::string> buttonNames = {
		{ GLFW_MOUSE_BUTTON_LEFT,     "Left" },
		{ GLFW_MOUSE_BUTTON_RIGHT,   "Right" },
		{ GLFW_MOUSE_BUTTON_MIDDLE, "Middle" },
		{ GLFW_MOUSE_BUTTON_4,      "Fourth" },
		{ GLFW_MOUSE_BUTTON_5,       "Fifth" },
		{ GLFW_MOUSE_BUTTON_6,       "Sixth" },
		{ GLFW_MOUSE_BUTTON_7,     "Seventh" },
		{ GLFW_MOUSE_BUTTON_8,        "Last" }
	};
	static const std::unordered_map<std::string, int> buttonCodes = {
		{ "Left",     GLFW_MOUSE_BUTTON_LEFT },
		{ "Right",   GLFW_MOUSE_BUTTON_RIGHT },
		{ "Middle", GLFW_MOUSE_BUTTON_MIDDLE },
		{ "Fourth",      GLFW_MOUSE_BUTTON_4 },
		{ "Fifth",       GLFW_MOUSE_BUTTON_5 },
		{ "Sixth",       GLFW_MOUSE_BUTTON_6 },
		{ "Seventh",     GLFW_MOUSE_BUTTON_7 },
		{ "Last",        GLFW_MOUSE_BUTTON_8 }
	};

	Keyboard::Keyboard() {
		for (usize i = 0; i < MAX_KEYS; i++) {
			keys[i] = Button();
		}
	}

	int Keyboard::ButtonCode(const std::string& name) {
		return keyCodes.find(name)->second;
	}

	std::string Keyboard::ButtonName(int code) {
		return keyNames.find(code)->second;
	}

	void Keyboard::KeyPressed(int key, int scancode, int action, int mods) {
		if (key != GLFW_KEY_UNKNOWN) {
			if (action == GLFW_PRESS) {
				keys[key].down = true;
			}
			else if (action == GLFW_REPEAT) {
				keys[key].repeat = true;
			}
			else {
				keys[key].down = false;
				keys[key].repeat = false;
			}
		}
	}

	void Keyboard::Prepare() {
		for (usize i = 0; i < MAX_KEYS; i++) {
			keys[i].last = keys[i].down;
		}
	}

	void Keyboard::PrepareTick() {
		for (usize i = 0; i < MAX_KEYS; i++) {
			keys[i].lastTick = keys[i].down;
		}
	}

	Mouse::Mouse() {
		for (usize i = 0; i < MAX_MOUSE_BUTTONS; i++) {
			buttons[i] = Button();
		}
	}

	int Mouse::ButtonCode(const std::string& name) {
		return buttonCodes.find(name)->second;
	}

	std::string Mouse::ButtonName(int code) {
		return buttonNames.find(code)->second;
	}

	void Mouse::Prepare() {
		for (usize i = 0; i < MAX_MOUSE_BUTTONS; i++) {
			buttons[i].last = buttons[i].last;
		}

		lastPos = pos;
		lastScroll = scroll;
	}

	void Mouse::PrepareTick() {
		for (usize i = 0; i < MAX_MOUSE_BUTTONS; i++) {
			buttons[i].lastTick = buttons[i].last;
		}

		lastPosTick = pos;
		lastScrollTick = scroll;
	}

	void Mouse::MouseMoved(const vec2& pos) {
		this->pos = pos;
	}

	void Mouse::Scrolled(const vec2& offset) {
		scroll += offset;
	}

	void Mouse::ButtonPressed(int button, int action, int mods) {
		if (action == GLFW_PRESS) {
			buttons[button].down = true;
		}
		else {
			buttons[button].down = false;
		}
	}

	void Mouse::Enter(int entered) {
		if (entered) {
			active = true;
		}
		else {
			active = false;
		}
	}
}