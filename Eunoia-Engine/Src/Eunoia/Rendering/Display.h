#pragma once

#include "DisplayInfo.h"
#include "../DataStructures/List.h"
#include "../Core/InputDefs.h"

namespace Eunoia {

	enum DisplayEventType
	{
		DISPLAY_EVENT_CREATE,
		DISPLAY_EVENT_CLOSE,
		DISPLAY_EVENT_RESIZE,
		DISPLAY_EVENT_KEY,
		DISPLAY_EVENT_MOUSE_BUTTON,
		NUM_DISPLAY_EVENT_TYPES
	};

	enum DisplayInputEventType
	{
		DISPLAY_INPUT_EVENT_PRESS,
		DISPLAY_INPUT_EVENT_RELEASE,

		NUM_DISPLAY_INPUT_EVENT_TYPES
	};

	enum DisplayAPI
	{
		EU_DISPLAY_API_WIN32,
		EU_DISPLAY_API_GLFW,
	};

	struct DisplayEvent
	{
		DisplayEventType type;
		u32 width; //Used for resize event.
		u32 height; //Used for resize event.
		u32 input; //Used for input event
		DisplayInputEventType inputType; //Used for input event
	};

	typedef void (*DisplayEventFunction)(const DisplayEvent&, void*);

	class EU_API Display
	{
	public:
		Display(DisplayAPI api);

		virtual b32 Init(const DisplayInfo& displayInfo) = 0;
		virtual void Destroy() = 0;
		virtual void Update() = 0;
		virtual void Present() = 0;
		
		virtual b32 CheckForEvent(DisplayEventType type) const = 0;

		virtual b32 IsMinimized() const = 0;
		virtual void GetSize(u32* w, u32* h) = 0;

		void AddDisplayEventCallback(DisplayEventFunction function, void* userPtr = 0);

		DisplayAPI GetDisplayAPI();

	protected:
		DisplayAPI m_DisplayAPI;
	protected:
		void ProcessDisplayEvents(const DisplayEvent& info);
		static void SetKeyState(Key key, b32 state);
		static void SetMouseButtonState(MouseButton button, b32 state);
	public:
		static Display* CreateDisplay();
	private:
		struct DisplayEventCallback
		{
			void* userPtr;
			DisplayEventFunction function;
		};

		List<DisplayEventCallback> m_Callbacks;
	};

}
