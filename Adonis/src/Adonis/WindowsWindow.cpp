#include "pch.h"
#include "WindowsWindow.h"

#include "Adonis/Eventsystem/EventManager.h"
#include "Adonis/Eventsystem/Events/Events.h"
#include <stb_image.h>


namespace Adonis {

	static bool s_glfw_initialized = false;
	static bool prev_vsync;

	static void GLFWErrorCallback(int error, const char* desc) {
		AD_CORE_ERROR("GLFW reported error: {0} with description: {1}", error, desc);
	}

	std::unique_ptr<IWindow> IWindow::create(const uint16_t& width, const uint16_t& height, const std::string& title, WindowMode mode, const bool& vsync) {
		return std::make_unique<WindowsWindow>(width, height, title, mode, vsync);
	}

	WindowsWindow::WindowsWindow(const uint16_t& width, const uint16_t& height, const std::string& title, WindowMode mode, const bool& vsync) :
		m_width(width),
		m_height(height),
		m_title(title),
		m_mode(mode),
		m_vsync(vsync){				
		init();
		prev_vsync = vsync;
	}

	WindowsWindow::~WindowsWindow() {
	}

	void WindowsWindow::init() {
		
		if (!s_glfw_initialized) {
			int status = glfwInit();
			AD_CORE_ASSERT(status, "Failed to initialize GLFW");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_glfw_initialized = true;
		}

		switch (m_mode) {
		case WindowMode::Windowed:
			m_window = std::unique_ptr<GLFWwindow, void(*)(GLFWwindow *)>(glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr), WindowsWindow::shutdown);
			break;
		case WindowMode::Borderless:
		{
			GLFWmonitor* monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* vid_mode = glfwGetVideoMode(monitor);
			glfwWindowHint(GLFW_RED_BITS, vid_mode->redBits);
			glfwWindowHint(GLFW_GREEN_BITS, vid_mode->greenBits);
			glfwWindowHint(GLFW_BLUE_BITS, vid_mode->blueBits);
			glfwWindowHint(GLFW_REFRESH_RATE, vid_mode->refreshRate);

			m_window = std::unique_ptr<GLFWwindow, void(*)(GLFWwindow *)>(glfwCreateWindow(vid_mode->width, vid_mode->height, m_title.c_str(), monitor, nullptr), glfwDestroyWindow);
		}
			break;
		case WindowMode::Fullscreen:
			m_window = std::unique_ptr<GLFWwindow, void(*)(GLFWwindow *)>(glfwCreateWindow(m_width, m_height, m_title.c_str(), glfwGetPrimaryMonitor(), nullptr), glfwDestroyWindow);
			break;
		default:break;
		}

		//Set member variables
		glfwGetFramebufferSize(m_window.get(), &m_framebuffer_width, &m_framebuffer_height);

		glfwMakeContextCurrent(m_window.get());
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		AD_CORE_ASSERT(status, "Failed to initialize GLAD");

		//set vsync
		#ifdef ADONIS_DEBUG
			AD_CORE_INFO("Setting up window with vsync status: {0}", m_vsync);
		#endif //ADONIS_DEBUG
		glfwSwapInterval(m_vsync);
		
		glfwSetKeyCallback(m_window.get(), [] (GLFWwindow* window, int key, int scancode, int action, int mods){
			switch (action) {
			case GLFW_PRESS:
				EventManager::queueEvent<KeyPressed>(key, mods);
				break;
			case GLFW_RELEASE:
				EventManager::queueEvent<KeyReleased>(key, mods);				
				break;
			case GLFW_REPEAT://TODO
				break;
			default: break;
			}
		});

		glfwSetMouseButtonCallback(m_window.get(), [](GLFWwindow*, int button, int action, int mods) {
			switch (action) {
			case GLFW_PRESS:
				EventManager::queueEvent<MouseButtonPressed>(button, mods);
				break;
			case GLFW_RELEASE:
				EventManager::queueEvent<MouseButtonReleased>(button, mods);
				break;
			case GLFW_REPEAT://TODO
				break;
			default: break;
			}
		});

		glfwSetCursorPosCallback(m_window.get(), [](GLFWwindow*, double x, double y) {
			EventManager::queueEvent<MouseMovedEvent>(x, y);
		});

		glfwSetScrollCallback(m_window.get(), [](GLFWwindow*, double xoffset, double yoffset) {
			EventManager::queueEvent<MouseScrolledEvent>(xoffset, yoffset);
		});

		glfwSetWindowSizeCallback(m_window.get(), [](GLFWwindow*, int width, int height) {
			EventManager::queueEvent<WindowResizeEvent>(width, height);
		});

		glfwSetWindowCloseCallback(m_window.get(), [](GLFWwindow*) {
			EventManager::queueEvent<WindowCloseEvent>();
		});

		glfwSetCharCallback(m_window.get(), [](GLFWwindow*, unsigned int c) {
			EventManager::queueEvent<CharTyped>(c);
		});

		//on_event<UpdateEvent>(  std::bind(&WindowsWindow::on_update, this, std::placeholders::_1));
		//on_event<PreRenderEvent>(  std::bind(&WindowsWindow::on_pre_render, this, std::placeholders::_1));
		//on_event<RenderEvent>(  std::bind(&WindowsWindow::on_render, this, std::placeholders::_1));
		//on_event<PostRenderEvent>( std::bind(&WindowsWindow::on_post_render, this, std::placeholders::_1));
		ON_EVENT_BIND(UpdateEvent, WindowsWindow, on_UpdateEvent);
		ON_EVENT_BIND(PreRenderEvent, WindowsWindow, on_PreRenderEvent);
		ON_EVENT_BIND(RenderEvent, WindowsWindow, on_RenderEvent);
		ON_EVENT_BIND(PostRenderEvent, WindowsWindow, on_PostRenderEvent);

		int width, height, n;
		unsigned char * data = stbi_load("WindowIcon.png", &width, &height, &n, 0);
		if (data == nullptr) {
			AD_CORE_ERROR("Failed to load window icon");
		}
		else {
			GLFWimage* image = new GLFWimage;
			image->height = height;
			image->width = width;
			image->pixels = data;
			glfwSetWindowIcon(m_window.get(), 1, image);
			delete image;
		}
		stbi_image_free(data);

	}

	void WindowsWindow::toggle_fullscreen() {
		auto monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		if (m_mode != WindowMode::Fullscreen) {
			glfwSetWindowMonitor(m_window.get(), monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
			m_mode = WindowMode::Fullscreen;
		}
		else {
			glfwSetWindowMonitor(m_window.get(), nullptr, 0, 30, m_width, m_height, mode->refreshRate);
			m_mode = WindowMode::Windowed;
		}
		glfwSwapInterval(m_vsync);
	}

	void WindowsWindow::on_UpdateEvent(const event_ptr_t<UpdateEvent>& ev) {
		glfwGetWindowSize(m_window.get(), &m_width, &m_height);
		glfwGetFramebufferSize(m_window.get(), &m_framebuffer_width, &m_framebuffer_height);
		glfwGetCursorPos(m_window.get(), &m_mouse_pos.x, &m_mouse_pos.y);
		if (prev_vsync != m_vsync) {
			AD_CORE_INFO("VSYNC STATUS CHANGED");
			update_vsync();
		}
		prev_vsync = m_vsync;
	}

	void WindowsWindow::shutdown(GLFWwindow* window) {
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void WindowsWindow::on_WindowCloseEvent(const event_ptr_t<WindowCloseEvent>& event) {
	}

	void WindowsWindow::on_PreRenderEvent(const event_ptr_t<PreRenderEvent>& event) {
		glfwPollEvents();
		glClearColor(0.2f, 0.7f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void WindowsWindow::on_RenderEvent(const event_ptr_t<RenderEvent>& event) {
		
	}

	void WindowsWindow::on_PostRenderEvent(const event_ptr_t<PostRenderEvent>& event) {
		glfwSwapBuffers(m_window.get());
	}

	bool WindowsWindow::has_focus()const {
		return static_cast<bool>(glfwGetWindowAttrib(m_window.get(), GLFW_FOCUSED));
	}

	void WindowsWindow::set_mouse_pos(double x, double y) {
		glfwSetCursorPos(m_window.get(), x, y);
	}

	bool& WindowsWindow::vsync() {
		return m_vsync;
	}

	void WindowsWindow::update_vsync() {
		glfwSwapInterval(m_vsync);
	}
}