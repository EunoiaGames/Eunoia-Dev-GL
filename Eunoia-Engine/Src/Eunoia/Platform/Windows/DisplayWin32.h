#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "../../Rendering/Display.h"

namespace Eunoia {


	typedef void (*DisplayProcFunction)(HWND, UINT, WPARAM, LPARAM);

	class EU_API DisplayWin32 : public Display
	{
	public:
		DisplayWin32();

		virtual b32 Init(const DisplayInfo& displayInfo) override;
		virtual void Destroy() override;
		virtual void Update() override;
		virtual void Present() override;

		virtual b32 CheckForEvent(DisplayEventType type) const override;

		virtual b32 IsMinimized() const override;
		virtual void GetSize(u32* w, u32* h);

		void AddWindowProc(WNDPROC proc);
		void AddWindowProc(DisplayProcFunction proc);

		HWND GetHandle() const;

		friend LRESULT CALLBACK DisplayCallbackWin32(HWND windowHandle, UINT msg, WPARAM wparam, LPARAM lparam);
		friend void HandleInput(WPARAM wparam, LPARAM lparam, DisplayWin32* display);
	private:
		void InitRawInputs();
	private:
		HWND m_Handle;
		HDC m_DeviceContext;
		u32 m_Width;
		u32 m_Height;

		WNDPROC m_ExtraWindowProcs[8];
		u32 m_NumExtraWindowProcs;

		List<DisplayProcFunction> m_Procs;

		b32 m_EventsThisFrame[NUM_DISPLAY_EVENT_TYPES];
	private:
		static LPCWSTR s_ClassName;
		static b32 s_ClassRegistered;
	};

}
