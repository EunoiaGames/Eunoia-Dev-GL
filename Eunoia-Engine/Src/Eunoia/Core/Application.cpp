#include "Application.h"
#include "../ECS/ECSLoader.h"

namespace Eunoia {
	Application::Application() :
		m_ActiveScene_(EU_INVALID_SCENE_ID)
	{}
	

	b32 Application::HasActiveScene()
	{
		return m_ActiveScene_ != EU_INVALID_SCENE_ID;
	}

	Scene* Application::GetActiveScene()
	{
		return &m_Scenes_[m_ActiveScene_ - 1];
	}

	ECS* Application::GetECS()
	{
		return m_Scenes_[m_ActiveScene_ - 1].ecs;
	}

	AssetManager* Application::GetAssetManager()
	{
		return m_Scenes_[m_ActiveScene_].assetManager;
	}

	SceneID Application::CreateScene(const String& name, b32 active)
	{
		Scene scene;
		scene.name = name;
		scene.path = "EU_NO_PATH";
		scene.assetManager = new AssetManager();
		scene.ecs = new ECS();
		scene.ecs->Init();
		scene.ecs->AddRequiredSystems();

		m_Scenes_.Push(scene);
		if(active)
			m_ActiveScene_ = m_Scenes_.Size();

		return m_Scenes_.Size();
	}

	SceneID Application::CreateScene(const String& path, const String& name, b32 active)
	{
		Scene scene;
		scene.name = name;
		scene.path = path;
		scene.assetManager = new AssetManager();
		scene.ecs = new ECS();
		scene.ecs->Init();

		ECSLoadedScene loadedScene;
		loadedScene.name = name;
		loadedScene.path = path;
		ECSLoader::SetAssetManager(scene.assetManager);
		ECSLoader::LoadECSSceneFromFile(&loadedScene, path);

		scene.ecs->InitFromLoadedSceneFormat(loadedScene);

		m_Scenes_.Push(scene);
		if (active)
			m_ActiveScene_ = m_Scenes_.Size();
		return m_Scenes_.Size();
	}

	void Application::SetActiveScene(const String& name)
	{
		for (u32 i = 0; i < m_Scenes_.Size(); i++)
		{
			if (m_Scenes_[i].name == name)
			{
				m_ActiveScene_ = i;
			}
		}
	}

	void Application::SetActiveScene(SceneID sceneID)
	{
		if (sceneID > m_Scenes_.Size())
			return;

		m_ActiveScene_ = sceneID;
	}

}
