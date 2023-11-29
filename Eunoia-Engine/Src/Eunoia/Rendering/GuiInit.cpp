#include "GuiInit.h"

#include "GuiInit.h"
#include "../../Vendor/imgui/imgui.h"
#include "../../Vendor/imgui/imgui_impl_win32.h"
#include "../../Vendor/imgui/imgui_impl_glfw.h"
#include "../../Vendor/imgui/imgui_impl_opengl3.h"

#include "../Platform/GLFW/DisplayGLFW.h"
#include "../Core/Engine.h"
#include "../Utils/Logger.h"
#include "../Core/Input.h"
#include "ImGuiInitTheme.h"

#ifdef EU_PLATFORM_WINDOWS
#include "../Platform/Windows/DisplayWin32.h"
#define WINDOWS_LEAN_AND_MEAN
#include <Windows.h>
#endif

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Eunoia {

	struct GUIData
	{
		ImGuiContext* context;
	};

	static GUIData s_GUI;

	static void DisplayEvent_UpdateImGuiInput(const Eunoia::DisplayEvent& eventInfo, void* userPtr)
	{
		if (eventInfo.type == Eunoia::DISPLAY_EVENT_KEY && eventInfo.inputType == Eunoia::DISPLAY_INPUT_EVENT_PRESS)
		{
			ImGuiIO& io = ImGui::GetIO();
			if (eventInfo.input == Eunoia::EU_KEY_BACKSPACE)
				io.AddKeyEvent(ImGuiKey_Backspace, true);
			else if (eventInfo.input == Eunoia::EU_KEY_ENTER)
				io.AddKeyEvent(ImGuiKey_Enter, true);
			else if (eventInfo.input == Eunoia::EU_KEY_SPACE)
			{
				io.AddKeyEvent(ImGuiKey_Space, true);
				io.AddInputCharacter(' ');
			}
			else
				io.AddInputCharacter(Eunoia::EUInput::GetChar((Eunoia::Key)eventInfo.input));

			GUIData* data = (GUIData*)userPtr;

		}
		if (eventInfo.type == Eunoia::DISPLAY_EVENT_KEY && eventInfo.inputType == Eunoia::DISPLAY_INPUT_EVENT_RELEASE)
		{
			ImGuiIO& io = ImGui::GetIO();
			if (eventInfo.input == Eunoia::EU_KEY_BACKSPACE)
				io.AddKeyEvent(ImGuiKey_Backspace, false);
			else if (eventInfo.input == Eunoia::EU_KEY_ENTER)
				io.AddKeyEvent(ImGuiKey_Enter, false);
			else if (eventInfo.input == Eunoia::EU_KEY_SPACE)
				io.AddKeyEvent(ImGuiKey_Space, false);

		}
	}

	void GuiInit::Init()
	{
		((DisplayWin32*)Engine::GetDisplay())->AddWindowProc(ImGui_ImplWin32_WndProcHandler);
		s_GUI.context = ImGui::CreateContext();
		ImGui::SetCurrentContext(s_GUI.context);
		Engine::GetDisplay()->AddDisplayEventCallback(DisplayEvent_UpdateImGuiInput);

		Display* display = Engine::GetDisplay();
		DisplayAPI dAPI = display->GetDisplayAPI();
		String msg;
		switch (dAPI)
		{
		case EU_DISPLAY_API_WIN32: {
			ImGui_ImplWin32_InitForOpenGL(((DisplayWin32*)display)->GetHandle());
			msg = "Win32";
		}break;
		case EU_DISPLAY_API_GLFW: {
			ImGui_ImplGlfw_InitForOpenGL(((DisplayGLFW*)display)->GetHandle(), true);
			msg = "GLFW";
		} break;
		}


		ImGui_ImplOpenGL3_Init("#version 330");

		ImGuiInitTheme::InitDark();
		ImGuiIO& io = ImGui::GetIO();
		io.FontDefault = io.Fonts->AddFontFromFileTTF("Res/Fonts/OpenSans-Regular.ttf", 18.0f);
		msg = "Initialized ImGui for (" + msg + ")";


		EU_LOG_INFO(msg.C_Str());
	}

	void GuiInit::Begin()
	{
		u32 width, height;
		Engine::GetDisplay()->GetSize(&width, &height);

		ImGui::SetCurrentContext(s_GUI.context);
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	void GuiInit::End()
	{
		ImGui::Render();
	}

	void GuiInit::Render()
	{
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}
