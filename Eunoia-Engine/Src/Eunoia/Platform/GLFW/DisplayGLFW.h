#pragma once

#include "../../Rendering/Display.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3.h"
#include "GLFW\glfw3native.h"

namespace Eunoia {

	class EU_API  DisplayGLFW : public Display
	{
	public:
		DisplayGLFW();
		virtual b32 Init(const DisplayInfo& displayInfo);
		virtual void Destroy() override;
		virtual void Update() override;
		virtual void Present() override;

		virtual b32 CheckForEvent(DisplayEventType type) const;

		virtual b32 IsMinimized() const;
		virtual void GetSize(u32* w, u32* h);

		GLFWwindow* GetHandle() const;

		friend void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		friend void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
		friend void framebuffer_resize_callback(GLFWwindow* window, int width, int height);
		friend void window_close_callback(GLFWwindow* window);
	private:
		GLFWwindow* m_Handle;
		b32 m_EventsThisFrame[NUM_DISPLAY_EVENT_TYPES];
		u32 m_Width;
		u32 m_Height;
	};

}