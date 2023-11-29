#pragma once

#include <Eunoia\Eunoia.h>

using namespace Eunoia;

class EditorGUI
{
public:
	static void Init();
	static void Begin();
	static void DrawGUI();
	static void End();
	static void DrawFrame();
};

