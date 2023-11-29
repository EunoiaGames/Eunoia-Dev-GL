#pragma once

#include "../Common.h"
#include "../Rendering/Renderer2D.h"
#include "../Rendering/Scene.h"
#include "../ECS/ECSLoader.h"

namespace Eunoia {

	class EU_API Application
	{
	public:
		Application();
		virtual void Init(Renderer2D* renderer2D) = 0;
		virtual void OnClose() {}

		virtual void PreUpdate(r32 dt) {}
		virtual void Update(r32 dt) {}
		virtual void PostUpdate(r32 dt) {}

		virtual void PreRender(Renderer2D* renderer2D) {}
		virtual void Render(Renderer2D* renderer2D) {}
		virtual void PostRender(Renderer2D* renderer2D) {}

		virtual void RegisterMetadata() {}
		virtual void OnRecompile() {}

		b32 HasActiveScene();
		Scene* GetActiveScene();

		ECS* GetECS();
		AssetManager* GetAssetManager();
	
		SceneID CreateScene(const String& name, b32 active = true);
		SceneID CreateScene(const String& path, const String& name, b32 active = true);
		void SetActiveScene(const String& name);
		void SetActiveScene(SceneID sceneID);
	public:
		List<Scene> m_Scenes_;
		u32 m_ActiveScene_;
	};

}
