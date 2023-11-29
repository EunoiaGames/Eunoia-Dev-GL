#include "ECSLoader.h"
#include "../Metadata/Metadata.h"
#include <cstdio>

namespace Eunoia {

	struct ECSLoader_Data
	{
		List<ECSLoaderUnsafeRuntimeMember> unsafeMembers;
		AssetManager* assetManager;
	};

	struct ECSLoaderMetadata
	{
		u32 numEntities;
		u32 numSystems;
	};

	struct ECSLoaderEntityMetadata
	{
		u32 numComponents;
		u32 numChildren;
	};

	static ECSLoader_Data s_Data;

	static void WriteTextureMember(List<u8>* safeRuntimeData, const void* unsafeRuntimeData)
	{
		String texturePath = s_Data.assetManager->GetTexturePath((*(TextureID*)unsafeRuntimeData));
		safeRuntimeData->SetCapacityAndElementCount(texturePath.Length() + sizeof(u32));
		u32 texturePathLength = texturePath.Length();
		memcpy(&safeRuntimeData->operator[](0), &texturePathLength, sizeof(u32));
		memcpy(&safeRuntimeData->operator[](sizeof(u32)), texturePath.C_Str(), texturePathLength);
	}

	static void ReadTextureMember(void* unsafeRuntimeData, const List<u8>& safeRuntimeData)
	{
		u32 texturePathLength;
		memcpy(&texturePathLength, &safeRuntimeData[0], sizeof(u32));
		String texturePath = String(texturePathLength);
		memcpy(texturePath.GetChars(), &safeRuntimeData[sizeof(u32)], texturePathLength);

		TextureID tid = s_Data.assetManager->LoadTexture(texturePath);
		memcpy(unsafeRuntimeData, &tid, sizeof(TextureID));
	}

	void ECSLoader::Init()
	{
		ECSLoaderUnsafeRuntimeMember unsafeMember;

		unsafeMember.typeName = "TextureID";
		unsafeMember.WriteUnsafeMember = WriteTextureMember;
		unsafeMember.ReadUnsafeMember = ReadTextureMember;
		AddUnsafeRuntimeMember(unsafeMember);
	}

	ECSLoadError ECSLoader::LoadECSSceneFromFile(ECSLoadedScene* loadedScene, const String& path)
	{
		FILE* file = fopen(path.C_Str(), "rb");
		if (!file)
			return ECS_LOAD_ERROR_FILE_NOT_FOUND;

		fseek(file, 0, SEEK_END);
		mem_size fileSize = ftell(file);
		fseek(file, 0, SEEK_SET);

		List<u8> memory(fileSize, fileSize);
		fread(&memory[0], 1, fileSize, file);
		ECSLoadError error = LoadECSSceneFromMemory(loadedScene, memory);
		loadedScene->path = path;
		fclose(file);
		return error;
	}

	ECSLoadError ECSLoader::LoadECSSceneFromMemory(ECSLoadedScene* loadedScene, const List<u8>& memory)
	{
		loadedScene->path = "EU_NO_PATH";
		const List<u8>& buffer = memory;
		mem_size offset = 0;

		char header[8];
		u8 version[3];
		ReadBuffer(header, buffer, &offset, 7);
		header[7] = 0;
		ReadBuffer(version, buffer, &offset, 3);

		if (strcmp(header, "euscene") != 0)
		{
			return ECS_LOAD_ERROR_NOT_EUECS_FILE;
		}
		if (!(version[0] == 1 && version[1] == 0 && version[2] == 0))
		{
			return ECS_LOAD_ERROR_UNSUPPORTED_VERSION;
		}

		ECSLoaderMetadata metadata;
		ReadBuffer(&metadata, buffer, &offset, sizeof(ECSLoaderMetadata));

		u32 length;
		ReadBuffer(&length, buffer, &offset, sizeof(u32));
		String sceneName(length);
		ReadBuffer(sceneName.GetChars(), buffer, &offset, length);
		loadedScene->name = sceneName;

		loadedScene->entities.SetCapacityAndElementCount(metadata.numEntities);
		loadedScene->systems.SetCapacityAndElementCount(metadata.numSystems);

		for (u32 i = 0; i < metadata.numEntities; i++)
		{
			ECSLoadedEntity* loadedEntity = &loadedScene->entities[i];

			ECSLoaderEntityMetadata entityMetadata;
			ReadBuffer(&entityMetadata, buffer, &offset, sizeof(ECSLoaderEntityMetadata));
			loadedEntity->numChildren = entityMetadata.numChildren;

			u32 length;
			ReadBuffer(&length, buffer, &offset, sizeof(u32));
			loadedEntity->name = String(length);
			ReadBuffer(loadedEntity->name.GetChars(), buffer, &offset, length);
			ReadBuffer(&loadedEntity->enabled, buffer, &offset, sizeof(b32));

			if (entityMetadata.numComponents > 0)
				loadedEntity->components.SetCapacityAndElementCount(entityMetadata.numComponents);

			for (u32 j = 0; j < entityMetadata.numComponents; j++)
			{
				ECSLoadedComponent* loadedComponent = &loadedEntity->components[j];
				ReadBuffer(&length, buffer, &offset, sizeof(u32));
				String componentTypeName(length);
				ReadBuffer(componentTypeName.GetChars(), buffer, &offset, length);
				loadedComponent->typeID = Metadata::GetClassTypeID(componentTypeName);
				ReadBuffer(&loadedComponent->enabled, buffer, &offset, sizeof(b32));

				MetadataClass* componentMetadata = Metadata::GetMetadata(loadedComponent->typeID).cls;
				mem_size componentDataSize = componentMetadata->size - componentMetadata->baseClassSize;
				if (componentDataSize > 0)
					loadedComponent->data.SetCapacityAndElementCount(componentDataSize);

				for (u32 k = 0; k < componentMetadata->members.Size(); k++)
				{
					const MetadataMember& componentMember = componentMetadata->members[k];
					b32 readUnsafeMember = false;
					for (u32 l = 0; l < s_Data.unsafeMembers.Size(); l++)
					{
						const ECSLoaderUnsafeRuntimeMember& unsafeMember = s_Data.unsafeMembers[l];
						if (componentMember.typeName == unsafeMember.typeName)
						{
							u32 safeDataSize;
							ReadBuffer(&safeDataSize, buffer, &offset, sizeof(u32));
							List<u8> safeData(safeDataSize, safeDataSize);
							ReadBuffer(&safeData[0], buffer, &offset, safeDataSize);
							unsafeMember.ReadUnsafeMember((void*)&loadedComponent->data[componentMember.offset - componentMetadata->baseClassSize], safeData);
							readUnsafeMember = true;
							break;
						}
					}

					if (!readUnsafeMember)
						ReadBuffer((void*)&loadedComponent->data[componentMember.offset - componentMetadata->baseClassSize], buffer, &offset, componentMember.size);
				}
			}
		}

		for (u32 i = 0; i < metadata.numSystems; i++)
		{
			ECSLoadedSystem* loadedSystem = &loadedScene->systems[i];
			ReadBuffer(&length, buffer, &offset, sizeof(u32));
			String systemTypeName(length);
			ReadBuffer(systemTypeName.GetChars(), buffer, &offset, length);
			ReadBuffer(&loadedSystem->enabled, buffer, &offset, sizeof(b32));

			loadedSystem->typeID = Metadata::GetClassTypeID(systemTypeName);
			MetadataClass* systemMetadata = Metadata::GetMetadata(loadedSystem->typeID).cls;
			loadedSystem->data.SetCapacityAndElementCount(systemMetadata->size);

			for (u32 j = 0; j < systemMetadata->members.Size(); j++)
			{
				const MetadataMember& systemMember = systemMetadata->members[j];
				b32 readUnsafeMember = false;
				for (u32 k = 0; k < s_Data.unsafeMembers.Size(); k++)
				{
					const ECSLoaderUnsafeRuntimeMember& unsafeMember = s_Data.unsafeMembers[k];
					if (systemMember.typeName == unsafeMember.typeName)
					{
						u32 safeDataSize;
						ReadBuffer(&safeDataSize, buffer, &offset, sizeof(u32));
						List<u8> safeData(safeDataSize, safeDataSize);
						ReadBuffer(&safeData[0], buffer, &offset, safeDataSize);
						unsafeMember.ReadUnsafeMember((void*)&loadedSystem->data[systemMember.offset - systemMetadata->baseClassSize], safeData);
						readUnsafeMember = true;
						break;
					}
				}

				if (!readUnsafeMember)
					ReadBuffer((void*)&loadedSystem->data[systemMember.offset - systemMetadata->baseClassSize], buffer, &offset, systemMember.size);
			}
		}

		char footer[8];
		ReadBuffer(footer, buffer, &offset, 5);
		footer[7] = 0;
		if (strcmp(footer, "euscene") != 0)
		{
			return ECS_LOAD_ERROR_UNFINISHED;
		}

		return ECS_LOAD_ERROR_NONE;
	}

	void ECSLoader::AddUnsafeRuntimeMember(const ECSLoaderUnsafeRuntimeMember& member)
	{
		s_Data.unsafeMembers.Push(member);
	}

	void ECSLoader::SetAssetManager(AssetManager* assetManager)
	{
		s_Data.assetManager = assetManager;
	}

	void ECSLoader::WriteLoadedSceneToFile(const ECSLoadedScene& loadedScene, const String& path)
	{
		FILE* file = fopen(path.C_Str(), "wb");

		char header[8] = "euscene";
		u8 version[3] = {1, 0, 0};
		fwrite(header, 1, 7, file);
		fwrite(version, 1, 3, file);

		ECSLoaderMetadata metadata;
		metadata.numSystems = loadedScene.systems.Size();
		metadata.numEntities = loadedScene.entities.Size();

		fwrite(&metadata, sizeof(ECSLoaderMetadata), 1, file);

		u32 length = loadedScene.name.Length();
		fwrite(&length, sizeof(u32), 1, file);
		fwrite(loadedScene.name.C_Str(), 1, length, file);

		for (u32 i = 0; i < loadedScene.entities.Size(); i++)
		{
			const ECSLoadedEntity& loadedEntity = loadedScene.entities[i];

			ECSLoaderEntityMetadata entityMetadata;
			entityMetadata.numComponents = loadedEntity.components.Size();
			entityMetadata.numChildren = loadedEntity.numChildren;
			fwrite(&entityMetadata, sizeof(ECSLoaderEntityMetadata), 1, file);

			length = loadedEntity.name.Length();
			fwrite(&length, sizeof(u32), 1, file);
			fwrite(loadedEntity.name.C_Str(), 1, length, file);
			fwrite(&loadedEntity.enabled, sizeof(b32), 1, file);

			for (u32 j = 0; j < loadedEntity.components.Size(); j++)
			{
				const ECSLoadedComponent& loadedComponent = loadedEntity.components[j];
				MetadataClass* componentMetadata = Metadata::GetMetadata(loadedComponent.typeID).cls;
				String componentTypeName = componentMetadata->name;
				length = componentTypeName.Length();
				fwrite(&length, sizeof(u32), 1, file);
				fwrite(componentTypeName.C_Str(), 1, length, file);
				fwrite(&loadedComponent.enabled, sizeof(b32), 1, file);

				for (u32 k = 0; k < componentMetadata->members.Size(); k++)
				{
					const MetadataMember& componentMember = componentMetadata->members[k];
					b32 wroteUnsafeMember = false;
					for (u32 l = 0; l < s_Data.unsafeMembers.Size(); l++)
					{
						const ECSLoaderUnsafeRuntimeMember& unsafeMember = s_Data.unsafeMembers[l];
						if (componentMember.typeName == unsafeMember.typeName)
						{
							List<u8> safeData;
							unsafeMember.WriteUnsafeMember(&safeData, (void*)&loadedComponent.data[componentMember.offset - componentMetadata->baseClassSize]);
							u32 safeDataSize = safeData.Size();
							fwrite(&safeDataSize, sizeof(u32), 1, file);
							fwrite(&safeData[0], 1, safeData.Size(), file);
							wroteUnsafeMember = true;
							break;
						}
					}

					if (!wroteUnsafeMember)
						fwrite((void*)&loadedComponent.data[componentMember.offset - componentMetadata->baseClassSize], 1, componentMember.size, file);
				}
			}
		}

		for (u32 i = 0; i < loadedScene.systems.Size(); i++)
		{
			const ECSLoadedSystem& loadedSystem = loadedScene.systems[i];
			MetadataClass* systemMetadata = Metadata::GetMetadata(loadedSystem.typeID).cls;
			String systemName = systemMetadata->name;
			u32 length = systemName.Length();
			fwrite(&length, sizeof(u32), 1, file);
			fwrite(systemName.C_Str(), 1, length, file);
			fwrite(&loadedSystem.enabled, sizeof(b32), 1, file);

			for (u32 j = 0; j < systemMetadata->members.Size(); j++)
			{
				const MetadataMember& systemMember = systemMetadata->members[j];
				b32 wroteUnsafeMember = false;
				for (u32 k = 0; k < s_Data.unsafeMembers.Size(); k++)
				{
					const ECSLoaderUnsafeRuntimeMember& unsafeMember = s_Data.unsafeMembers[k];
					if (systemMember.typeName == unsafeMember.typeName)
					{
						List<u8> safeData;
						unsafeMember.WriteUnsafeMember(&safeData, (void*)&loadedSystem.data[systemMember.offset - systemMetadata->baseClassSize]);
						u32 safeDataSize = safeData.Size();
						fwrite(&safeDataSize, sizeof(u32), 1, file);
						fwrite(&safeData[0], 1, safeData.Size(), file);
						wroteUnsafeMember = true;
						break;
					}
				}

				if (!wroteUnsafeMember)
					fwrite((void*)&loadedSystem.data[systemMember.offset - systemMetadata->baseClassSize], 1, systemMember.size, file);
			}
		}

		fwrite(header, 1, 7, file);

		fclose(file);
	}

	void ECSLoader::ReadBuffer(void* dst, const List<u8>& buffer, mem_size* offset, mem_size size)
	{
		memcpy(dst, &buffer[*offset], size);
		*offset += size;
	}

}
