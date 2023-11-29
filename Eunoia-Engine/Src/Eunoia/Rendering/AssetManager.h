#pragma once

#include "RenderAPI.h"
#include "../DataStructures/List.h"

namespace Eunoia {

	struct TextureAsset
	{
		TextureID id;
		String path;
	};

	class EU_API AssetManager
	{
	public:
		TextureID LoadTexture(const String& path);
		void FreeResources();
		void FreeTextures();

		String GetTexturePath(TextureID textureID);
	private:
		List<TextureAsset> m_Textures;
	};

}