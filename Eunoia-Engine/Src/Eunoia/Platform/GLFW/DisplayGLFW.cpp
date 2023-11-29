#include "DisplayGLFW.h"
#include "../../Utils/Logger.h"
#include "../../Core/Engine.h"

namespace Eunoia {

	DisplayGLFW::DisplayGLFW() :
		Display(EU_DISPLAY_API_GLFW)
	{
		memset(m_EventsThisFrame, false, sizeof(b32) * NUM_DISPLAY_EVENT_TYPES);
	}

	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		DisplayGLFW* display = (DisplayGLFW*)glfwGetWindowUserPointer(window);

		DisplayEvent ev;
		ev.type = DISPLAY_EVENT_KEY;

		switch (action)
		{
			case GLFW_PRESS: {
				display->SetKeyState((Key)scancode, true);
				ev.inputType = DISPLAY_INPUT_EVENT_PRESS;
				display->m_EventsThisFrame[DISPLAY_EVENT_KEY];
				break;
			}
			case GLFW_RELEASE: {
				display->SetKeyState((Key)scancode, false);
				ev.inputType = DISPLAY_INPUT_EVENT_RELEASE;
				display->m_EventsThisFrame[DISPLAY_EVENT_KEY];
				break;
			}
		}

		display->ProcessDisplayEvents(ev);
	}

	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
	{
		DisplayGLFW* display = (DisplayGLFW*)glfwGetWindowUserPointer(window);

		DisplayEvent ev;
		ev.type = DISPLAY_EVENT_MOUSE_BUTTON;

		switch (action)
		{
		case GLFW_PRESS: {
			display->SetMouseButtonState((MouseButton)button, true);
			ev.inputType = DISPLAY_INPUT_EVENT_PRESS;
			display->m_EventsThisFrame[DISPLAY_EVENT_MOUSE_BUTTON];
			break;
		}
		case GLFW_RELEASE: {
			display->SetMouseButtonState((MouseButton)button, false);
			ev.inputType = DISPLAY_INPUT_EVENT_RELEASE;
			display->m_EventsThisFrame[DISPLAY_EVENT_MOUSE_BUTTON];
			break;
		}
		}

		display->ProcessDisplayEvents(ev);
	}

	void framebuffer_resize_callback(GLFWwindow* window, int width, int height)
	{
		DisplayEvent ev;

		DisplayGLFW* display = (DisplayGLFW*)glfwGetWindowUserPointer(window);
		display->m_Width = width;
		display->m_Height = height;

		ev.width = width;
		ev.height = height;
		ev.type = DISPLAY_EVENT_RESIZE;

		display->m_EventsThisFrame[DISPLAY_EVENT_RESIZE] = true;

		display->ProcessDisplayEvents(ev);
	}

	void window_close_callback(GLFWwindow* window)
	{
		DisplayGLFW* display = (DisplayGLFW*)glfwGetWindowUserPointer(window);

		DisplayEvent ev;
		ev.type = DISPLAY_EVENT_CLOSE;

		display->m_EventsThisFrame[DISPLAY_EVENT_CLOSE] = true;

		display->ProcessDisplayEvents(ev);
	}

	b32 DisplayGLFW::Init(const DisplayInfo& displayInfo)
	{
		if(!glfwInit())
		{
			EU_LOG_ERROR("Could not initialize GLFW");
			return false;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

		u32 width, height;
		switch (displayInfo.res)
		{
		case EU_DISPLAY_RESOLUTION_16_9: width = 16 * displayInfo.resScale; height = 9 * displayInfo.resScale; break;
		case EU_DISPLAY_RESOLUTION_4_3: width = 4 * displayInfo.resScale; height = 3 * displayInfo.resScale; break;
		}

		m_Handle = glfwCreateWindow(width, height, displayInfo.title.C_Str(), EU_NULL, EU_NULL);

		glfwSetWindowUserPointer(m_Handle, this);
		glfwSetKeyCallback(m_Handle, key_callback);
		glfwSetMouseButtonCallback(m_Handle, mouse_button_callback);
		glfwSetFramebufferSizeCallback(m_Handle, framebuffer_resize_callback);
		glfwSetWindowCloseCallback(m_Handle, window_close_callback);

		if (!m_Handle)
		{
			EU_LOG_ERROR("Could not create GLFW window");
			return false;
		}

		return true;
	}

	void DisplayGLFW::Destroy()
	{
		glfwDestroyWindow(m_Handle);
	}

	void DisplayGLFW::Update()
	{
		memset(m_EventsThisFrame, false, sizeof(b32) * NUM_DISPLAY_EVENT_TYPES);
		glfwPollEvents();
	}

	void DisplayGLFW::Present()
	{
		glfwSwapBuffers(m_Handle);
	}

	b32 DisplayGLFW::CheckForEvent(DisplayEventType type) const
	{
		return m_EventsThisFrame[type];
	}

	b32 DisplayGLFW::IsMinimized() const
	{
		return glfwGetWindowAttrib(m_Handle, GLFW_ICONIFIED);
	}

	void DisplayGLFW::GetSize(u32* w, u32* h)
	{
		s32 wi, he;
		glfwGetFramebufferSize(m_Handle, &wi, &he);
		*w = wi;
		*h = he;
	}

	GLFWwindow* DisplayGLFW::GetHandle() const
	{
		return m_Handle;
	}
}
