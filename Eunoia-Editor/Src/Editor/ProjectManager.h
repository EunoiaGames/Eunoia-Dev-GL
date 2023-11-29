#pragma once

#include <Eunoia\Eunoia.h>

struct EunoiaProject
{
	Eunoia::String name;
	b32 loaded;
	HMODULE dllHandle;
	Eunoia::Application* application;
	Eunoia::EUDirectory* assetDirectory;

	b32 stepApplication;
	b32 stepPaused;
};

enum SystemFunctionFlags
{
	SYSTEM_FUNCTION_PreUpdate = 1,
	SYSTEM_FUNCTION_Update = 2,
	SYSTEM_FUNCTION_PostUpdate = 4,
	SYSTEM_FUNCTION_PreRender = 8,
	SYSTEM_FUNCTION_Render = 16,
	SYSTEM_FUNCTION_PostRender = 32,
};

class ProjectManager
{
public:
	static void Init();

	static b32 CreateNewProject(const Eunoia::String& name);
	static b32 LoadProject(const Eunoia::String& name, b32 compile);
	static b32 RecompileProject();
	static b32 SaveProject();
	static b32 DeleteProject(const Eunoia::String& name);
	static b32 CreateFinalBuild();

	static EunoiaProject* GetProject();

	static void OpenVS2019();

	static const Eunoia::List<Eunoia::String>& GetProjectNamesInProjectFolder();
private:
	static void OpenVS2019Helper();
};