#include "Window.h"
#include "Util\Configuration.h"
#include "Input.h"
#include "Util\Log.h"

static GLFWmonitor* WindowMonitor(GLFWwindow* window, const ivec2& windowPos, const uvec2& windowSize) {
	int count;
	GLFWmonitor** monitors = glfwGetMonitors(&count);
	GLFWmonitor* closestMonitor = NULL;
	u32 closestMonitorArea = 0;
	for (int i = 0; i < count; i++) {
		GLFWmonitor* monitor = monitors[i];
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		int x, y;
		glfwGetMonitorPos(monitor, &x, &y);
		
		ivec2 windowMin = windowPos;
		ivec2 windowMax = windowPos + static_cast<ivec2>(windowSize);
		ivec2 monitorMin(x, y);
		ivec2 monitorMax(x + mode->width, y + mode->height);

		ivec2 overlapMin = math::Max(windowMin, monitorMin);
		ivec2 overlapMax = math::Min(windowMax, monitorMax);
		ivec2 overlap = overlapMax - overlapMin;

		u32 overlapArea = overlap.x * overlap.y;
		if (overlapArea > closestMonitorArea) {
			closestMonitorArea = overlapArea;
			closestMonitor = monitor;
		}
	}

	return closestMonitor;
}

namespace platform {

	u32 Window::numWindows = 0;

	Window::Window(const std::string& name, uvec2 size, u32 monitorIndex, bool fullscreen) : name(name), size(size), fullscreen(fullscreen) {
		if (++numWindows == 1) {
			if (glfwInit() == GLFW_FALSE) {
				const char* msg;
				int error = glfwGetError(&msg);
				ErrorCallback(error, msg);

				return;
			}

			glfwSetErrorCallback(&Window::ErrorCallback);
		}

		int numMonitors;
		GLFWmonitor** monitors = glfwGetMonitors(&numMonitors);
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		if (monitorIndex >= numMonitors) {
			global.log->LogNoFile("Invalid monitor index ($)!", util::Logger::GLFW, util::Logger::Warn, monitorIndex);
		}
		else {
			monitor = monitors[monitorIndex];
		}

		if (fullscreen) {
			this->monitor = monitor;
		}

		const GLFWvidmode* vidMode = glfwGetVideoMode(monitor);

		ivec2 monitorPos;
		glfwGetMonitorPos(monitor, &monitorPos.x, &monitorPos.y);

		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

		window = glfwCreateWindow(size.x, size.y, name.c_str(), fullscreen ? monitor : NULL, NULL);
		if (window == NULL) {
			return;
		}

		glfwSetWindowUserPointer(window, static_cast<void*>(this));

		glfwSetWindowPosCallback(window, &Window::MoveCallback);
		glfwSetWindowSizeCallback(window, &Window::ResizeCallback);
		glfwSetFramebufferSizeCallback(window, &Window::FramebufferResizeCallback);
		glfwSetWindowRefreshCallback(window, &Window::RefreshCallback);
		glfwSetKeyCallback(window, &Window::KeyCallback);
		glfwSetCursorPosCallback(window, &Window::MouseCallback);
		glfwSetCursorEnterCallback(window, &Window::MouseEnterCallback);
		glfwSetMouseButtonCallback(window, &Window::MouseButtonCallback);

		glfwDefaultWindowHints();

		pos = monitorPos + ivec2(vidMode->width - size.x, vidMode->height - size.y) / 2;
		glfwSetWindowPos(window, pos.x, pos.y);

		glfwGetFramebufferSize(window, (int*)&fullSize.x, (int*)&fullSize.y);

		cursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
		if (!cursor) {
			global.log->LogNoFile("Failed to create cursor", util::Logger::GLFW, util::Logger::Warn);
		}
		else {
			glfwSetCursor(window, cursor);
		}

		glfwShowWindow(window);
	}

	Window::~Window() {
		if (cursor) {
			glfwDestroyCursor(cursor);
		}
		if (window) {
			glfwDestroyWindow(window);
		}

		if (--numWindows == 0) {
			glfwTerminate();
		}
	}

	bool Window::IsMinimized() const {
		return size.x == 0 || size.y == 0;
	}

	bool Window::ShouldClose() const {
		return glfwWindowShouldClose(window) || glfwGetKey(window, global.config->exit);
	}

	void Window::Close() {
		glfwSetWindowShouldClose(window, true);
	}

	void Window::ToggleFullscreen() {
		fullscreen = !fullscreen;
		if (fullscreen) {
			monitor = WindowMonitor(window, pos, size);

			const GLFWvidmode* mode = glfwGetVideoMode(monitor);
			oldPos = pos;
			oldSize = size;

			glfwSetWindowMonitor(window, monitor, GLFW_DONT_CARE, GLFW_DONT_CARE, mode->width, mode->height, mode->refreshRate);

			size = uvec2(mode->width, mode->height);
			glfwGetMonitorPos(monitor, &pos.x, &pos.y);
			glfwGetFramebufferSize(window, (int*)&fullSize.x, (int*)&fullSize.y);
		}
		else {
			monitor = NULL;

			glfwSetWindowMonitor(window, NULL, oldPos.x, oldPos.y, oldSize.x, oldSize.y, GLFW_DONT_CARE);

			pos = oldPos;
			size = oldSize;

			glfwGetFramebufferSize(window, (int*)&fullSize.x, (int*)&fullSize.y);
		}
	}

	VkResult Window::CreateSurface(VkInstance instance, const VkAllocationCallbacks* allocator, VkSurfaceKHR* surface) const {
		return glfwCreateWindowSurface(instance, window, allocator, surface);
	}

	void Window::ErrorCallback(int error, const char* msg) {
		global.log->LogNoFile("GLFW Error $ ($)!", util::Logger::GLFW, util::Logger::Error, error, msg);
	}

	void Window::MoveCallback(GLFWwindow* window, int xpos, int ypos) {
		Window* ptr = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		ivec2 pos(xpos, ypos);
		ptr->pos = pos;
	}

	void Window::ResizeCallback(GLFWwindow* window, int width, int height) {
		Window* ptr = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		uvec2 size(static_cast<u32>(width), static_cast<u32>(height));
		ptr->size = size;
	}

	void Window::FramebufferResizeCallback(GLFWwindow* window, int width, int height) {
		Window* ptr = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		ptr->fullSize = uvec2(static_cast<u32>(width), static_cast<u32>(height));
	}

	void Window::RefreshCallback(GLFWwindow* window) {
		//TODO: redraw the screen
	}

	void Window::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		Window* ptr = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		ptr->keyboard->KeyPressed(key, scancode, action, mods);
	}

	void Window::MouseEnterCallback(GLFWwindow* window, int entered) {
		Window* ptr = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		ptr->mouse->active = static_cast<b8>(entered);
	}

	void Window::MouseCallback(GLFWwindow* window, double xpos, double ypos) {
		Window* ptr = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		ptr->mouse->MouseMoved(vec2(xpos, ypos));
	}

	void Window::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
		Window* ptr = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		ptr->mouse->Scrolled(vec2(xoffset, yoffset));
	}

	void Window::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
		Window* ptr = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		ptr->mouse->ButtonPressed(button, action, mods);
	}
}