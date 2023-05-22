#pragma once

#include "Util\GLFW.h"
#include "Util\Types.h"
#include "Util\Std.h"
#include "Util\Math.h"

namespace util {

	struct Controls;
}

namespace platform {

	class Keyboard;
	class Mouse;

	class Window {
	public:
		Window(const std::string& name, uvec2 size, u32 monitorIndex = 0, bool fullscreen = false);
		~Window();

		//Non copyable, non movable
		Window(const Window& other) = delete;
		Window& operator=(const Window& other) = delete;
		Window(Window&& other) noexcept = delete;
		Window& operator=(Window&& other) noexcept = delete;

		uvec2 Size() const { return size; }
		uvec2 FullSize() const { return fullSize; }
		const std::string& Name() const { return name; }
		ivec2 Pos() const { return pos; }

		bool IsMinimized() const;
		bool IsResized() const { return resized; }
		void ResetFlags() { resized = false; }
		bool ShouldClose() const;
		void Close();

		void ToggleFullscreen();
		inline bool Fullscreen() const { return fullscreen; }

		VkResult CreateSurface(VkInstance instance, const VkAllocationCallbacks* allocator, VkSurfaceKHR* surface) const;

	private:
		//Callbacks
		static void ErrorCallback(int error, const char* msg);
		static void MoveCallback(GLFWwindow* window, int xpos, int ypos);
		static void ResizeCallback(GLFWwindow* window, int width, int height);
		static void FramebufferResizeCallback(GLFWwindow* window, int width, int height);
		static void RefreshCallback(GLFWwindow* window);
		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void MouseEnterCallback(GLFWwindow* window, int entered);
		static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
		static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		static void FocusCallback(GLFWwindow* window, int action);

		GLFWwindow* window;
		GLFWmonitor* monitor;
		GLFWcursor* cursor;

		std::string name;
		uvec2 size; //Size in screen coords
		uvec2 fullSize; //Size in pixels

		ivec2 pos;

		b8 fullscreen;
		ivec2 oldPos;
		uvec2 oldSize;

		b8 resized = false;

		Keyboard* keyboard;
		Mouse* mouse;

		static u32 numWindows;

		friend struct Platform;
	};
}