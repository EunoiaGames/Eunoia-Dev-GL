#include "ProjectManager.h"

#include <filesystem>
#include <thread>

using namespace Eunoia;

typedef Eunoia::Application* (*CreateApplicationFunction) (Eunoia::ApplicationInfo* appInfo);

struct ProjectManager_Data
{
	EunoiaProject project;
	std::thread thread;
	List<String> projectNames;
};

static ProjectManager_Data s_ProjectManager;

void ProjectManager::Init()
{
	s_ProjectManager.project.loaded = false;
	s_ProjectManager.project.stepApplication = false;
	s_ProjectManager.project.stepPaused = false;

	std::filesystem::directory_iterator it("../EunoiaProjects");
	for (const auto& entry : it)
	{
		if (!entry.is_directory())
			continue;
		Eunoia::String projPath = Eunoia::String(entry.path().generic_string().c_str());
		s_ProjectManager.projectNames.Push(projPath.SubString(projPath.FindLastOf("/") + 1));
	}
}

b32 ProjectManager::CreateNewProject(const Eunoia::String& name)
{
	EunoiaProject* project = &s_ProjectManager.project;
	project->name = name;

	const String rootProjectDir = "../EunoiaProjects/";

	String projectSrcDir = rootProjectDir + name + "/Src";
	String mkdirCmd = "mkdir \"" + projectSrcDir + "\"";
	std::system(mkdirCmd.C_Str());

	mkdirCmd = "mkdir \"" + projectSrcDir + "/Components\"";
	std::system(mkdirCmd.C_Str());
	mkdirCmd = "mkdir \"" + projectSrcDir + "/Systems\"";
	std::system(mkdirCmd.C_Str());
	mkdirCmd = "mkdir \"" + projectSrcDir + "/Events\"";
	std::system(mkdirCmd.C_Str());

	String headerFileText = "#pragma once\n#include <Eunoia\\Eunoia.h>\n\nusing namespace Eunoia;\n\nclass EU_PROJ " + name +
		" : public Application\n{\npublic:\n\tvoid Init(Renderer2D* renderer2D) override;\n\tvoid OnClose() override;\n\tvoid RegisterMetadata() override;"
		"\n\n};\n\nextern \"C\" EU_PROJ Application* CreateApplication(ApplicationInfo* appInfo);";

	String sourceFileText = "#include \"" + name + ".h\"\n\nvoid " + name + "::Init(Renderer2D* renderer2D)\n{\n\n}\n\nvoid " + name +
		"::OnClose()\n{\n\n}\n\nApplication* CreateApplication(ApplicationInfo* appInfo)\n{\n\tappInfo->version = {1, 0, 0};\n\n\tappInfo->name = \"" + name + "\";\n\tappInfo->displayInfo.title = \""
		+ name + "\";\n\tappInfo->displayInfo.res = EU_DISPLAY_RESOLUTION_16_9;\n\tappInfo->displayInfo.resScale = 80;\n\n\treturn new " + name + "();\n}";

	String genFileText = "#include \"" + name + ".h\"";

	Eunoia::String location = projectSrcDir + "/" + name + ".h";
	FILE* file = fopen(location.C_Str(), "w");
	fwrite(headerFileText.C_Str(), 1, headerFileText.Length(), file);
	fclose(file);

	location = projectSrcDir + "/" + name + ".cpp";
	file = fopen(location.C_Str(), "w");
	fwrite(sourceFileText.C_Str(), 1, sourceFileText.Length(), file);
	fclose(file);

	location = projectSrcDir + "/" + name + "_Generated.cpp";
	file = fopen(location.C_Str(), "w");
	fwrite(genFileText.C_Str(), 1, genFileText.Length(), file);
	fclose(file);

	Eunoia::String genProjectFilesCommand = "call GenProjectFiles-VS2019.bat \"" + name + "\"";
	std::system(genProjectFilesCommand.C_Str());

	Eunoia::String configuration = "Debug";
	Eunoia::String buildFolder = "Debug-windows-x86_64";

	Eunoia::String compileProjectCommand = "call CompileProj.bat ../EunoiaProjects/" + name + "/" + name + ".vcxproj " + configuration;
	std::system(compileProjectCommand.C_Str());

	String dllPath = "../EunoiaProjects/" + name + "/Bin/" + buildFolder + "/" + name + ".dll";
	project->dllHandle = LoadLibraryA(dllPath.C_Str());
	if (!project->dllHandle)
	{
		EU_LOG_ERROR("Could not load project DLL");
		return false;
	}

	CreateApplicationFunction createApplication = (CreateApplicationFunction)GetProcAddress(project->dllHandle, "CreateApplication");
	if (!createApplication)
	{
		EU_LOG_ERROR("Could not load CreateApplication function from project dll");
		return false;
	}

	EU_LOG_INFO("Loaded Project");

	ApplicationInfo appInfo;

	project->loaded = true;
	project->stepPaused = false;
	project->stepApplication = false;
	project->application = createApplication(&appInfo);
}

b32 ProjectManager::LoadProject(const Eunoia::String& name, b32 compile)
{
	EunoiaProject* project = &s_ProjectManager.project;

	b32 found = false;
	for (u32 i = 0; i < s_ProjectManager.projectNames.Size(); i++)
	{
		if (name == s_ProjectManager.projectNames[i])
		{
			found = true;
			break;
		}
	}

	if (!found)
	{
		EU_LOG_ERROR("Tried to load a project that doesn't exist");
		return false;
	}

	String configuration = "Debug";
	String buildFolder = "Debug-windows-x86_64";

	if (compile)
	{
		String genProjectFilesCommand = "call GenProjectFiles-VS2019.bat \"" + project->name + "\"";
		std::system(genProjectFilesCommand.C_Str());

		String CompileProjectCommand = "call CompileProj.bat ../EunoiaProjects/" + name + "/" + name + ".vcxproj " + configuration;
		std::system(CompileProjectCommand.C_Str());
	}

	if (project->loaded)
	{
		FreeLibrary(project->dllHandle);
		project->dllHandle = 0;
	}

	String dllPath = "../EunoiaProjects/" + name + "/Bin/" + buildFolder + "/" + name + ".dll";
	project->dllHandle = LoadLibraryA(dllPath.C_Str());
	if (!project->dllHandle)
	{
		EU_LOG_ERROR("Could not load project DLL");
		return false;
	}

	CreateApplicationFunction createApplication = (CreateApplicationFunction)GetProcAddress(project->dllHandle, "CreateApplication");
	if (!createApplication)
	{
		EU_LOG_ERROR("Could not load CreateApplication function from project dll");
		return false;
	}


	Eunoia::ApplicationInfo appInfo;
	project->application = createApplication(&appInfo);
	project->application->Init(Engine::GetRenderer2D());
	Eunoia::Metadata::UnregisterProjectMetadata();
	project->application->RegisterMetadata();

	project->name = name;
	project->loaded = true;
	project->stepApplication = false;
	project->stepPaused = false;
	project->assetDirectory = new EUDirectory("../EunoiaProjects/" + name + "/Assets");

	EU_LOG_INFO("Loaded project");

	return true;
}

b32 ProjectManager::RecompileProject()
{
	EunoiaProject* project = &s_ProjectManager.project;
	if (!project->loaded)
	{
		EU_LOG_WARN("Tried to recompile project before loading one first");
		return false;
	}

	String configuration = "Debug";
	String buildFolder = "Debug-windows-x86_64";

	String genProjectFilesCommand = "call GenProjectFiles-VS2019.bat \"" + project->name + "\"";
	std::system(genProjectFilesCommand.C_Str());

	FreeLibrary(project->dllHandle);
	Eunoia::String compileProjectCommand = "call CompileProj.bat ../EunoiaProjects/" + project->name + "/" + project->name + ".vcxproj " + configuration;
	std::system(compileProjectCommand.C_Str());

	Eunoia::String dllPath = "../EunoiaProjects/" + project->name + "/Bin/" + buildFolder + "/" + project->name + ".dll";
	project->dllHandle = LoadLibraryA(dllPath.C_Str());
	if (!project->dllHandle)
	{
		EU_LOG_ERROR("Could not load project DLL");
		return false;
	}

	CreateApplicationFunction createApplication = (CreateApplicationFunction)GetProcAddress(project->dllHandle, "CreateApplication");
	if (!createApplication)
	{
		EU_LOG_ERROR("Could not load CreateApplication function from project dll");
		return false;
	}

	//project->application->m_ECS->CreateResetPoint(&project->resetPoint);

	// s_Project.application->GetECS();
	//delete s_Project.application;

	Eunoia::ApplicationInfo appInfo;
	project->application = createApplication(&appInfo);
	Eunoia::Metadata::UnregisterProjectMetadata();
	project->application->RegisterMetadata();
	//project->application->m_ECS->RestoreResetPoint(project->resetPoint);
	project->application->OnRecompile();

	EU_LOG_INFO("Recompiled project");

	return true;
}

b32 ProjectManager::SaveProject()
{
	EunoiaProject* project = &s_ProjectManager.project;
	if (!project->loaded)
		return false;

	Application* app = project->application;
	for (u32 i = 0; i < app->m_Scenes_.Size(); i++)
	{
		Scene* scene = &app->m_Scenes_[i];
		ECSLoadedScene loadedScene;
		loadedScene.name = scene->name;
		loadedScene.path = scene->path;
	
		scene->ecs->ConvertToLoadedSceneFormat(&loadedScene);
		if (loadedScene.path != "EU_NO_PATH")
		{
			ECSLoader::SetAssetManager(scene->assetManager);
			ECSLoader::WriteLoadedSceneToFile(loadedScene, loadedScene.path);
		}
	}
}

b32 ProjectManager::DeleteProject(const Eunoia::String& name)
{
	return b32();
}

b32 ProjectManager::CreateFinalBuild()
{
	return b32();
}

EunoiaProject* ProjectManager::GetProject()
{
	return &s_ProjectManager.project;
}

void ProjectManager::OpenVS2019()
{
	s_ProjectManager.thread = std::thread(OpenVS2019Helper);
	s_ProjectManager.thread.detach();
}

const Eunoia::List<Eunoia::String>& ProjectManager::GetProjectNamesInProjectFolder()
{
	return s_ProjectManager.projectNames;
}

void ProjectManager::OpenVS2019Helper()
{
	if (!s_ProjectManager.project.loaded)
	{
		EU_LOG_WARN("Tried to open VS2019 before loading project");
		return;
	}

	Eunoia::String openVisualStudioCommand = "call \"../EunoiaProjects/" + s_ProjectManager.project.name + "/" + s_ProjectManager.project.name + ".sln\"";
	std::system(openVisualStudioCommand.C_Str());
}
