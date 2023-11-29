#include "DisplayWin32.h"
#include "../../Utils/Logger.h"

namespace Eunoia {

	LPCWSTR DisplayWin32::s_ClassName = L"EUNOIA";
	b32 DisplayWin32::s_ClassRegistered = false;

	DisplayWin32::DisplayWin32() :
		Display(EU_DISPLAY_API_WIN32),
		m_NumExtraWindowProcs(0)
	{
		memset(m_EventsThisFrame, false, sizeof(b32) * NUM_DISPLAY_EVENT_TYPES);
	}

	b32 DisplayWin32::Init(const DisplayInfo& displayInfo)
	{

		HINSTANCE instance = GetModuleHandle(0);

		if (!s_ClassRegistered)
		{
			WNDCLASS wndclass = {};
			wndclass.style = CS_OWNDC;
			wndclass.hInstance = instance;
			wndclass.lpszClassName = s_ClassName;
			wndclass.lpfnWndProc = DisplayCallbackWin32;
			wndclass.hCursor = LoadCursor(0, IDC_ARROW);

			if (!RegisterClass(&wndclass))
			{
				EU_LOG_FATAL("Could not register win32 class");
				return false;
			}

			s_ClassRegistered = true;
		}

		const String& title = displayInfo.title;
		r32 resScale = displayInfo.resScale;
		u32 width, height;

		switch (displayInfo.res)
		{
		case EU_DISPLAY_RESOLUTION_16_9: width = resScale * 16; height = resScale * 9; break;
		case EU_DISPLAY_RESOLUTION_4_3: width = resScale * 4; height = resScale * 3; break;
		}

		wchar_t wStringTitle[1024];
		MultiByteToWideChar(CP_ACP, 0, title.C_Str(), -1, wStringTitle, title.Length());
		wStringTitle[title.Length()] = 0;

		u32 flags = WS_VISIBLE | WS_SYSMENU | WS_BORDER | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;

		m_Handle = CreateWindowEx(0, s_ClassName, wStringTitle, flags,
			CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, 0, (LPVOID)this);

		if (!m_Handle)
		{
			EU_LOG_FATAL("Could not create Win32 display");
			return false;
		}

		InitRawInputs();

		PIXELFORMATDESCRIPTOR pixel_format = {};

		pixel_format.dwFlags = PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;

		pixel_format.nVersion = 1;
		pixel_format.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pixel_format.cColorBits = 32;
		pixel_format.cRedBits = 8;
		pixel_format.cGreenBits = 8;
		pixel_format.cBlueBits = 8;
		pixel_format.cAlphaBits = 8;
		pixel_format.cDepthBits = 24;
		pixel_format.cStencilBits = 8;
		pixel_format.iPixelType = PFD_TYPE_RGBA;
		pixel_format.cAuxBuffers = 0;
		pixel_format.iLayerType = PFD_MAIN_PLANE;

		m_DeviceContext = GetDC(m_Handle);
		int pixelFormat = ChoosePixelFormat(m_DeviceContext, &pixel_format);
		if (pixelFormat)
		{
			if (!SetPixelFormat(m_DeviceContext, pixelFormat, &pixel_format))
			{
				EU_LOG_FATAL("Could not set pixel format for win32");
				return false;
			}
		}
		else
		{
			EU_LOG_FATAL("Could not set pixel format for win32");
			return false;
		}

		RECT size;
		GetClientRect(m_Handle, &size);
		m_Width = size.right - size.left;
		m_Height = size.bottom - size.top;

		EU_LOG_INFO("Created Win32 display");
		return true;
	}

	void DisplayWin32::Destroy()
	{
		DestroyWindow(m_Handle);
	}

	void Eunoia::DisplayWin32::Update()
	{
		memset(m_EventsThisFrame, false, sizeof(b32) * NUM_DISPLAY_EVENT_TYPES);

		MSG msg = {};
		while (PeekMessage(&msg, m_Handle, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	void DisplayWin32::Present()
	{
		SwapBuffers(m_DeviceContext);
	}

	b32 DisplayWin32::CheckForEvent(DisplayEventType type) const
	{
		return m_EventsThisFrame[type];
	}

	b32 DisplayWin32::IsMinimized() const
	{
		return IsIconic(m_Handle);
	}

	void DisplayWin32::GetSize(u32* w, u32* h)
	{
		*w = m_Width;
		*h = m_Height;
	}

	void DisplayWin32::AddWindowProc(WNDPROC proc)
	{
		m_ExtraWindowProcs[m_NumExtraWindowProcs++] = proc;
	}

	void DisplayWin32::AddWindowProc(DisplayProcFunction proc)
	{
		m_Procs.Push(proc);
	}

	HWND DisplayWin32::GetHandle() const
	{
		return m_Handle;
	}

	void DisplayWin32::InitRawInputs()
	{
		RAWINPUTDEVICE keyboard = {};
		keyboard.dwFlags = RIDEV_NOLEGACY;
		keyboard.usUsagePage = 0x01;
		keyboard.usUsage = 0x06;
		keyboard.hwndTarget = m_Handle;

		RAWINPUTDEVICE mouse = {};
		mouse.dwFlags = 0;
		mouse.usUsagePage = 0x01;
		mouse.usUsage = 0x02;
		mouse.hwndTarget = m_Handle;

		RAWINPUTDEVICE inputDevices[2] = { mouse, keyboard };

		BOOL result = RegisterRawInputDevices(inputDevices, 2, sizeof(RAWINPUTDEVICE));
		if (!result)
		{
			EU_LOG_ERROR("Could not initialize Win32 RawInputDevices");
			return;
		}
	}

	LRESULT DisplayCallbackWin32(HWND windowHandle, UINT msg, WPARAM wparam, LPARAM lparam)
	{


		DisplayWin32* display = 0;
		LRESULT result = 0;

		DisplayEvent e{};

		if (msg != WM_CREATE)
			display = (Eunoia::DisplayWin32*)GetWindowLongPtr(windowHandle, GWLP_USERDATA);

		switch (msg)
		{
		case WM_INPUT: {
			HandleInput(wparam, lparam, display);
		} break;
		case WM_CREATE: {
			SetWindowLongPtr(windowHandle, GWLP_USERDATA, (LONG_PTR)((CREATESTRUCT*)lparam)->lpCreateParams);
			display = (Eunoia::DisplayWin32*)((CREATESTRUCT*)lparam)->lpCreateParams;
			display->m_EventsThisFrame[DISPLAY_EVENT_CREATE] = true;
			e.type = DISPLAY_EVENT_CREATE;
			display->ProcessDisplayEvents(e);
		} break;
		case WM_SIZE: {
			display->m_EventsThisFrame[DISPLAY_EVENT_RESIZE] = true;
			display->m_Width = LOWORD(lparam);
			display->m_Height = HIWORD(lparam);
			e.width = display->m_Width;
			e.height = display->m_Height;
			e.type = DISPLAY_EVENT_RESIZE;
			display->ProcessDisplayEvents(e);
		} break;
		case WM_CLOSE: {
			display->m_EventsThisFrame[DISPLAY_EVENT_CLOSE] = true;
		} break;
		default: {
			result = DefWindowProc(windowHandle, msg, wparam, lparam);
		}
		}

		if (display)
		{
			for (u32 i = 0; i < display->m_NumExtraWindowProcs; i++)
				display->m_ExtraWindowProcs[i](windowHandle, msg, wparam, lparam);

			for (u32 i = 0; i < display->m_Procs.Size(); i++)
				display->m_Procs[i](windowHandle, msg, wparam, lparam);
		}

		return result;
	}

	void HandleInput(WPARAM wparam, LPARAM lparam, DisplayWin32* display)
	{
		RAWINPUT* input;
		UINT size = 0;
		LPBYTE bytes;

		GetRawInputData((HRAWINPUT)lparam, RID_INPUT, NULL, &size, sizeof(RAWINPUTHEADER));

		if (size == 0)
			return;

		bytes = new BYTE[size];

		GetRawInputData((HRAWINPUT)lparam, RID_INPUT, bytes, &size, sizeof(RAWINPUTHEADER));

		input = (RAWINPUT*)bytes;

		DisplayEvent e;

		if (input->header.dwType == RIM_TYPEKEYBOARD)
		{
			e.type = DISPLAY_EVENT_KEY;
			if (input->data.keyboard.Message == WM_KEYDOWN)
			{
				e.inputType = DISPLAY_INPUT_EVENT_PRESS;
				USHORT key = input->data.keyboard.MakeCode;
				e.input = (u32)key;
				Display::SetKeyState((Key)key, true);
				display->m_EventsThisFrame[DISPLAY_EVENT_KEY] = true;
			}
			else if (input->data.keyboard.Message == WM_KEYUP)
			{
				e.inputType = DISPLAY_INPUT_EVENT_RELEASE;
				USHORT key = input->data.keyboard.MakeCode;
				e.input = (u32)key;
				Display::SetKeyState((Key)key, false);
				display->m_EventsThisFrame[DISPLAY_EVENT_KEY] = true;
			}
		}
		else if (input->header.dwType == RIM_TYPEMOUSE)
		{
			const RAWMOUSE& mouse = input->data.mouse;

			USHORT wheel = mouse.usButtonData;
			USHORT button = mouse.ulButtons;

			u32 engineButton = 0;
			s32 buttonStatus = -1;

			switch (button)
			{
			case RI_MOUSE_BUTTON_1_DOWN:	engineButton = 0; buttonStatus = true; break;
			case RI_MOUSE_BUTTON_1_UP:		engineButton = 0; buttonStatus = false; break;
			case RI_MOUSE_BUTTON_2_DOWN:	engineButton = 1; buttonStatus = true; break;
			case RI_MOUSE_BUTTON_2_UP:		engineButton = 1; buttonStatus = false; break;
			case RI_MOUSE_BUTTON_3_DOWN:	engineButton = 2; buttonStatus = true; break;
			case RI_MOUSE_BUTTON_3_UP:		engineButton = 2; buttonStatus = false; break;
			case RI_MOUSE_BUTTON_4_DOWN:	engineButton = 3; buttonStatus = true; break;
			case RI_MOUSE_BUTTON_4_UP:		engineButton = 3; buttonStatus = false; break;
			case RI_MOUSE_BUTTON_5_DOWN:	engineButton = 4; buttonStatus = true; break;
			case RI_MOUSE_BUTTON_5_UP:		engineButton = 4; buttonStatus = false; break;
			case RI_MOUSE_WHEEL:			engineButton = 5; buttonStatus = true; break;
			}

			if (buttonStatus != -1)
			{
				Display::SetMouseButtonState((MouseButton)engineButton, buttonStatus);

				e.type = DISPLAY_EVENT_MOUSE_BUTTON;
				e.input = engineButton;
				if (buttonStatus)
					e.inputType = DISPLAY_INPUT_EVENT_PRESS;
				else
					e.input = DISPLAY_INPUT_EVENT_RELEASE;

				display->m_EventsThisFrame[DISPLAY_EVENT_MOUSE_BUTTON] = true;
			}
		}

		display->ProcessDisplayEvents(e);

		delete[] bytes;
	}
}
