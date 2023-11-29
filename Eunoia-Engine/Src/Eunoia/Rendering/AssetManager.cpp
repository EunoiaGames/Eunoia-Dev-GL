#include "AssetManager.h"
#include "../Core/Engine.h"

namespace Eunoia {

	TextureID AssetManager::LoadTexture(const String& path)
	{
		TextureID tex = Engine::GetRenderAPI()->CreateTexture(path);
		TextureAsset asset;
		asset.id = tex;
		asset.path = path;
		m_Textures.Push(asset);
		return tex;
	}

	void AssetManager::FreeResources()
	{
		FreeTextures();
	}

	void AssetManager::FreeTextures()
	{
		for (u32 i = 0; i < m_Textures.Size(); i++)
		{
			Engine::GetRenderAPI()->DestroyTexture(m_Textures[i].id);
		}
	}

	String AssetManager::GetTexturePath(TextureID textureID)
	{
		for (u32 i = 0; i < m_Textures.Size(); i++)
		{
			if (m_Textures[i].id == textureID)
				return m_Textures[i].path;
		}

		return "Not Found";
	}

}
