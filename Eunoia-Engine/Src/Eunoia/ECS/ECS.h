#pragma once

#include "ECSTypes.h"
#include "../Metadata/Metadata.h"
#include "../Utils/Logger.h"
#include "../DataStructures/Map.h"
#include "ECSLoader.h"

#define EU_ECS_ROOT_ENTITY 1

/*
	Root entity cannot be destroyed
	Components shouldn't be added to the root entity, but you can if you want.
*/

namespace Eunoia {

	EU_REFLECT()
	enum ECSSystemProcessType
	{
		EU_ECS_SYSTEM_PROCESS_UPDATE,
		EU_ECS_SYSTEM_PROCESS_RENDER,
	};

	EU_REFLECT()
	struct ECSComponent
	{
		virtual ~ECSComponent() {}
		virtual void OnDestroy() {}

		EU_PROPERTY() b32 enabled;
		EU_PROPERTY() EntityID parent;
	};

	class ECS;
	EU_REFLECT()
	class ECSSystem
	{
	public:
		ECSSystem() :
			m_NumRequiredComponents(0),
			enabled(true)
		{}

		virtual void Init() {}

		virtual ~ECSSystem() {}

		virtual void PreUpdate(r32 dt) {}
		virtual void ProcessEntityOnUpdate(EntityID entity, r32 dt) {}
		virtual void PostUpdate(r32 dt) {}

		virtual void PreRender() {}
		virtual void ProcessEntityOnRender(EntityID entity) {}
		virtual void PostRender() {}

		EU_PROPERTY()
		b32 enabled;
	protected:
		template<class C>
		inline void AddComponentType() { m_RequiredComponets[m_NumRequiredComponents++] = Metadata::GetTypeID<C>(); }

		friend class ECS;
		ECS* m_ECS;
		metadata_typeid m_RequiredComponets[EU_ECS_MAX_COMPONENTS_A_SYSTEM_CAN_PROCESS];
		u32 m_NumRequiredComponents;
	};

	struct ECSComponentContainer
	{
		ECSComponent* actualComponent;
		metadata_typeid typeID;
		u32 allocatorIndex;
	};

	typedef List<ECSComponentContainer> ComponentList;

	struct ECSEntityContainer
	{
		String name;
		b32 enabled;
		ComponentList components;
		List<EntityID> children;
		EntityID parent;
		List<SystemIndex> compatibleSystems;
		b32 checkedForCompatibleSystems;
		b32 initialized;
	};

	struct ECSSystemContainer
	{
		metadata_typeid typeID;
		ECSSystem* actualSystem;
		u32 allocatorIndex;
	};

	class ECS
	{
	public:
		ECS() :
			m_SystemAllocator(512, 32)
		{}

		inline void Init()
		{
			ECSEntityContainer root;
			root.enabled = true;
			root.name = "Root";
			root.parent = EU_ECS_INVALID_ENTITY_ID;
			root.checkedForCompatibleSystems = false;

			m_Entities.Push(root);
		}

		inline void Begin()
		{

		}

		inline List<ECSSystemContainer>& GetSystems_() { return m_Systems; }
		inline List<ECSEntityContainer>& GetAllEntities_() { return m_Entities; }
		inline List<ECSSystemContainer>& GetAllSystems_() { return m_Systems; }

		EU_API void AddRequiredSystems();

		inline EntityID CreateEntity(const String& name, b32 enabled = true, EntityID parent = EU_ECS_ROOT_ENTITY)
		{
			ECSEntityContainer container;
			container.name = name;
			container.enabled = enabled;
			container.checkedForCompatibleSystems = false;
			container.initialized = true;
			
			if (parent == EU_ECS_INVALID_ENTITY_ID)
			{
				EU_LOG_ERROR("parent entity must be valid");
				return EU_ECS_INVALID_ENTITY_ID;
			}

			container.parent = parent;
			ECSEntityContainer* parentContainer = &m_Entities[parent - 1];

			if (m_FreeEntityIDs.Empty())
			{
				m_Entities.Push(container);
				EntityID entityID = m_Entities.Size();
				parentContainer->children.Push(entityID);
				return entityID;
			}
			
			EntityID entityID = m_FreeEntityIDs.GetLastElement();
			m_FreeEntityIDs.Pop();
			m_Entities[entityID - 1] = container;
			parentContainer->children.Push(entityID);

			return entityID;
		}

		inline void DestroyEntity(EntityID entity)
		{
			if (entity == EU_ECS_INVALID_ENTITY_ID)
			{
				EU_LOG_WARN("Tried to delete an invalid entity ID");
				return;
			}
			else if (entity == EU_ECS_ROOT_ENTITY)
			{
				EU_LOG_WARN("Tried to delete root entity, this is not allowed");
				return;
			}

			ECSEntityContainer* container = &m_Entities[entity - 1];
			container->initialized = false;
			for (u32 i = 0; i < container->components.Size(); i++)
			{
				DestroyComponentByIndex(entity, i);
			}

			m_FreeEntityIDs.Push(entity);
		}

		inline EntityID GetParentEntity(EntityID entity)
		{
			return m_Entities[entity - 1].parent;
		}

		inline void SetEntityEnabled(EntityID entity, b32 enabled = true)
		{
			m_Entities[entity - 1].enabled = enabled;
		}

		inline void SetEntityEnabledOpposite(EntityID entity)
		{
			m_Entities[entity - 1].enabled = !m_Entities[entity - 1].enabled;
		}

		inline void SetEntityName(EntityID entity, const String& name)
		{
			m_Entities[entity - 1].name = name;
		}

		inline const String& GetEntityName(EntityID entity)
		{
			return m_Entities[entity - 1].name;
		}

		template<class C>
		inline C* CreateComponent(EntityID entity, const C& init = C(), b32 enabled = true)
		{
			if (entity == EU_ECS_INVALID_ENTITY_ID)
			{
				EU_LOG_WARN("Tried to create a component on an invalid entity");
				return EU_NULL;
			}

			metadata_typeid componentTypeID = Metadata::GetTypeID<C>();

			ECSEntityContainer* entityContainer = &m_Entities[entity - 1];
			entityContainer->checkedForCompatibleSystems = false;

			ECSComponentContainer componentContainer;
			componentContainer.typeID = componentTypeID;
			
			DynamicPoolAllocator* allocator = EU_NULL;
			b32 foundAllocator = m_ComponentAllocators.FindElement(componentTypeID, &allocator);
			if (!foundAllocator)
			{
				allocator = new DynamicPoolAllocator(sizeof(C), 16);
			}

			componentContainer.actualComponent = (ECSComponent*)allocator->Allocate(&componentContainer.allocatorIndex);
			new(componentContainer.actualComponent) C(init);
			componentContainer.actualComponent->enabled = enabled;
			componentContainer.actualComponent->parent = entity;

			entityContainer->components.Push(componentContainer);
			return (C*)componentContainer.actualComponent;
		}

		inline ECSComponent* CreateComponent(EntityID entity, metadata_typeid componentTypeID, b32 enabled = true)
		{
			if (entity == EU_ECS_INVALID_ENTITY_ID)
			{
				EU_LOG_WARN("Tried to create a component on an invalid entity");
				return EU_NULL;
			}

			ECSEntityContainer* entityContainer = &m_Entities[entity - 1];
			entityContainer->checkedForCompatibleSystems = false;

			ECSComponentContainer componentContainer;
			componentContainer.typeID = componentTypeID;

			MetadataClass* metadata = Metadata::GetMetadata(componentTypeID).cls;

			DynamicPoolAllocator* allocator = EU_NULL;
			b32 foundAllocator = m_ComponentAllocators.FindElement(componentTypeID, &allocator);
			if (!foundAllocator)
			{
				allocator = new DynamicPoolAllocator(metadata->size, 16);
			}

			componentContainer.actualComponent = (ECSComponent*)allocator->Allocate(&componentContainer.allocatorIndex);
			Metadata::CallDefaultConstructor(componentTypeID, componentContainer.actualComponent);
			componentContainer.actualComponent->enabled = enabled;
			componentContainer.actualComponent->parent = entity;

			entityContainer->components.Push(componentContainer);
			return componentContainer.actualComponent;
		}

		template<class C>
		inline void DestroyComponent(EntityID entity)
		{
			metadata_typeid componentTypeID = Metadata::GetTypeID<C>();
			DestroyComponent(entity, componentTypeID);
		}

		inline b32 DestroyComponent(EntityID entity, metadata_typeid componentTypeID)
		{
			DynamicPoolAllocator* allocator = m_ComponentAllocators[componentTypeID];
			if (!allocator)
			{
				EU_LOG_WARN("Tried to delete a component of a type that has not been allocated yet");
				return false;
			}

			ECSEntityContainer* entityContainer = &m_Entities[entity - 1];
			entityContainer->checkedForCompatibleSystems = false;

			for (u32 i = 0; i < entityContainer->components.Size(); i++)
			{
				ECSComponentContainer* componentContainer = &entityContainer->components[i];
				if (componentTypeID == componentContainer->typeID)
				{
					ECSComponent* component = componentContainer->actualComponent;
					component->OnDestroy();
					allocator->Free(component, componentContainer->allocatorIndex);
					entityContainer->components.Remove(i);
					return true;
				}
			}
		}

		inline b32 DestroyComponentByIndex(EntityID entity, u32 componentIndex)
		{
			if (entity == EU_ECS_INVALID_ENTITY_ID)
				return false;

			ECSEntityContainer* entityContainer = &m_Entities[entity - 1];
			entityContainer->checkedForCompatibleSystems = false;
			ECSComponentContainer* component = &entityContainer->components[componentIndex];
			DynamicPoolAllocator* allocator;
			m_ComponentAllocators.FindElement(component->typeID, &allocator);
			if (!allocator)
				return false;

			component->actualComponent->OnDestroy();
			allocator->Free(component->actualComponent, component->allocatorIndex);
			entityContainer->components.Remove(componentIndex);
			return true;
		}

		template<class C>
		inline C* GetComponent(EntityID entity)
		{
			metadata_typeid componentTypeID = Metadata::GetTypeID<C>();
			return (C*)GetComponent(entity, componentTypeID);
		}

		inline ECSComponent* GetComponent(EntityID entity, metadata_typeid componentTypeID)
		{
			ECSEntityContainer* entityContainer = &m_Entities[entity - 1];
			return GetComponent(entityContainer->components, componentTypeID);
		}

		inline ECSComponent* GetComponent(const ComponentList& components, metadata_typeid componentTypeID)
		{
			for (u32 i = 0; i < components.Size(); i++)
			{
				if (components[i].typeID == componentTypeID)
				{
					return components[i].actualComponent;
				}
			}
		}

		template<class C>
		inline void SetComponentEnabled(EntityID entity, b32 enabled = true)
		{
			SetComponentEnabled(entity, Metadata::GetTypeID<C>(), enabled);
		}

		inline void SetComponentEnabled(EntityID entity, metadata_typeid componentTypeID, b32 enabled = true)
		{
			ECSEntityContainer* entityContainer = &m_Entities[entity - 1];
			for (u32 i = 0; i < entityContainer->components.Size(); i++)
			{
				if (entityContainer->components[i].typeID == componentTypeID)
				{
					entityContainer->components[i].actualComponent->enabled = enabled;
					return;
				}
			}
		}

		template<class C>
		inline void SetComponentEnabledOpposite(EntityID entity)
		{
			SetComponentEnabledOpposite(entity, Metadata::GetTypeID<C>());
		}

		inline void SetComponentEnabledOpposite(EntityID entity, metadata_typeid componentTypeID)
		{
			ECSEntityContainer* entityContainer = &m_Entities[entity - 1];
			for (u32 i = 0; i < entityContainer->components.Size(); i++)
			{
				if (entityContainer->components[i].typeID == componentTypeID)
				{
					entityContainer->components[i].actualComponent->enabled = !entityContainer->components[i].actualComponent->enabled;
					return;
				}
			}
		}

		template<class S>
		inline S* CreateSystem(b32 enabled = true)
		{
			return (S*)CreateSystem(Metadata::GetTypeID<S>(), enabled);
		}

		inline ECSSystem* CreateSystem(metadata_typeid systemTypeID, b32 enabled = true)
		{	
			ECSSystemContainer systemContainer;
			systemContainer.typeID = systemTypeID;

			if (Metadata::GetMetadata(systemTypeID).cls->size > 512)
			{
				EU_LOG_FATAL("This system is to big. TODO: Allow for bigger systems...");
				return 0;
			}

			systemContainer.actualSystem = (ECSSystem*)m_SystemAllocator.Allocate(&systemContainer.allocatorIndex);
			Metadata::CallDefaultConstructor(systemTypeID, systemContainer.actualSystem);

			systemContainer.actualSystem->m_ECS = this;
			systemContainer.actualSystem->enabled = enabled;
			systemContainer.actualSystem->Init();

			m_Systems.Push(systemContainer);

			for (u32 i = 0; i < m_Entities.Size(); i++)
			{
				ECSEntityContainer* entityContainer = &m_Entities[i];
				CheckSystemEntityCompatibility(entityContainer, m_Systems.Size() - 1);
			}

			return systemContainer.actualSystem;
		}

		template<class S>
		inline void DestroySystem()
		{
			DestroySystem(Metadata::GetTypeID<S>());
		}

		inline b32 DestroySystem(metadata_typeid systemTypeID)
		{
			for (u32 i = 0; i < m_Systems.Size(); i++)
			{
				if (systemTypeID == m_Systems[i].typeID)
				{
					m_SystemAllocator.Free(m_Systems[i].actualSystem, m_Systems[i].allocatorIndex);
					m_Systems.Remove(i);
					return true;
				}
			}

			for (u32 i = 0; i < m_Entities.Size(); i++)
			{
				ECSEntityContainer* entity = &m_Entities[i];
				entity->compatibleSystems.Clear();
				entity->checkedForCompatibleSystems = false;
			}

			return false;
		}

		template<class S>
		inline void SetSystemEnabled(b32 enabled)
		{
			SetSystemEnabled(Metadata::GetTypeID<S>(), enabled);
		}

		inline void SetSystemEnabled(metadata_typeid systemTypeID, b32 enabled)
		{
			for (u32 i = 0; i < m_Systems.Size(); i++)
			{
				if (systemTypeID == m_Systems[i].typeID)
				{
					m_Systems[i].actualSystem->enabled = enabled;
					return;
				}
			}
		}

		template<class S>
		inline void SetSystemEnabledOpposite()
		{
			SetSystemEnabledOpposite(Metadata::GetTypeID<S>());
		}

		inline void SetSystemEnabledOpposite(metadata_typeid systemTypeID)
		{
			for (u32 i = 0; i < m_Systems.Size(); i++)
			{
				if (systemTypeID == m_Systems[i].typeID)
				{
					m_Systems[i].actualSystem->enabled = !m_Systems[i].actualSystem->enabled;
					return;
				}
			}
		}



		inline void UpdateEntities(r32 dt)
		{
			ProcessEntities(EU_ECS_SYSTEM_PROCESS_UPDATE, dt);
		}

		inline void RenderEntities()
		{
			ProcessEntities(EU_ECS_SYSTEM_PROCESS_RENDER, 0.0f);
		}

		EU_API void UpdateTransformHierarchySystems(r32 dt);

		inline void ProcessEntities(ECSSystemProcessType processType, r32 dt)
		{
			for (u32 i = 0; i < m_Systems.Size(); i++)
			{
				ECSSystemContainer* system = &m_Systems[i];

				if (!system->actualSystem->enabled)
					continue;

				switch (processType)
				{
					case EU_ECS_SYSTEM_PROCESS_UPDATE: system->actualSystem->PreUpdate(dt); break;
					case EU_ECS_SYSTEM_PROCESS_RENDER: system->actualSystem->PreRender(); break;
				}
			}

			ProcessEntity(EU_ECS_ROOT_ENTITY, processType, dt);

			for (u32 i = 0; i < m_Systems.Size(); i++)
			{
				ECSSystemContainer* system = &m_Systems[i];

				if (!system->actualSystem->enabled)
					continue;

				switch (processType)
				{
				case EU_ECS_SYSTEM_PROCESS_UPDATE: system->actualSystem->PostUpdate(dt); break;
				case EU_ECS_SYSTEM_PROCESS_RENDER: system->actualSystem->PostRender(); break;
				}
			}
		}

		inline void InitFromLoadedSceneFormat(const ECSLoadedScene& loadedScene)
		{
			for (u32 i = 0; i < loadedScene.systems.Size(); i++)
			{
				const ECSLoadedSystem& loadedSystem = loadedScene.systems[i];
				ECSSystem* system = CreateSystem(loadedSystem.typeID, loadedSystem.enabled);
				MetadataClass* systemMetadata = Metadata::GetMetadata(loadedSystem.typeID).cls;
				mem_size systemSize = systemMetadata->size - sizeof(ECSSystem);
				if (systemSize > 0 && !systemMetadata->members.Empty())
					memcpy((u8*)system + sizeof(ECSSystem), &loadedSystem.data[0], systemSize);
				system->enabled = loadedSystem.enabled;
				system->Init();
			}

			if (!loadedScene.entities.Empty())
			{
				u32 entityIndex = 0;
				LoadSceneEntityFromLoadedDataFormat(loadedScene.entities, entityIndex, EU_ECS_INVALID_ENTITY_ID);
			}
		}

		inline void LoadSceneEntityFromLoadedDataFormat(const List<ECSLoadedEntity>& loadedEntities, u32& entityIndex, EntityID parent)
		{
			const ECSLoadedEntity& loadedEntity = loadedEntities[entityIndex];

			EntityID entity;
			if (entityIndex == 0)
				entity = EU_ECS_ROOT_ENTITY;
			else
				entity = CreateEntity(loadedEntity.name, true, parent);

			SetEntityEnabled(entity, loadedEntity.enabled);

			for (u32 i = 0; i < loadedEntity.components.Size(); i++)
			{
				const ECSLoadedComponent& loadedComponent = loadedEntity.components[i];
				ECSComponent* component = CreateComponent(entity, loadedComponent.typeID);
				mem_size componentSize = Metadata::GetMetadata(loadedComponent.typeID).cls->size - sizeof(ECSComponent);
				if (componentSize > 0)
					memcpy((u8*)component + sizeof(ECSComponent), &loadedComponent.data[0], componentSize);
				component->enabled = loadedComponent.enabled;
			}

			entityIndex++;

			for (u32 i = 0; i < loadedEntity.numChildren; i++)
			{
				LoadSceneEntityFromLoadedDataFormat(loadedEntities, entityIndex, entity);
			}
		}

		inline void ConvertToLoadedSceneFormat(ECSLoadedScene* loadedScene)
		{
			loadedScene->entities.Clear();
			loadedScene->systems.Clear();

			for (u32 i = 0; i < m_Systems.Size(); i++)
			{
				const ECSSystemContainer& system = m_Systems[i];
				ECSLoadedSystem loadedSystem;
				loadedSystem.enabled = system.actualSystem->enabled;
				loadedSystem.typeID = system.typeID;

				mem_size systemSize = Metadata::GetMetadata(system.typeID).cls->size;
				mem_size dataSize = systemSize - sizeof(ECSSystem);
				if (dataSize > 0)
				{
					loadedSystem.data.SetCapacityAndElementCount(systemSize);
					memcpy(&loadedSystem.data[0], (u8*)system.actualSystem + sizeof(ECSSystem), dataSize);
				}

				loadedScene->systems.Push(loadedSystem);
			}

			ECSEntityContainer* rootEntity = &m_Entities[EU_ECS_ROOT_ENTITY - 1];
			ConvertEntityToLoadedEntityFormat(loadedScene, rootEntity);
		}

		inline void ConvertEntityToLoadedEntityFormat(ECSLoadedScene* loadedScene, ECSEntityContainer* entity)
		{
			ECSLoadedEntity loadedEntity;
			loadedEntity.name = entity->name;
			loadedEntity.enabled = entity->enabled;
			loadedEntity.numChildren = entity->children.Size();

			for (u32 i = 0; i < entity->components.Size(); i++)
			{
				ECSComponentContainer* component = &entity->components[i];
				ECSLoadedComponent loadedComponent;
				loadedComponent.typeID = component->typeID;

				mem_size componentSize = Metadata::GetMetadata(component->typeID).cls->size;
				mem_size dataSize = componentSize - sizeof(ECSComponent);
				if (dataSize > 0)
				{
					loadedComponent.data.SetCapacityAndElementCount(dataSize);
					memcpy(&loadedComponent.data[0], (u8*)component->actualComponent + sizeof(ECSComponent), dataSize);
				}

				loadedEntity.components.Push(loadedComponent);
			}

			loadedScene->entities.Push(loadedEntity);

			for (u32 i = 0; i < entity->children.Size(); i++)
				ConvertEntityToLoadedEntityFormat(loadedScene, &m_Entities[entity->children[i] - 1]);
		}
	private:
		inline void ProcessEntity(EntityID entity, ECSSystemProcessType processType, r32 dt)
		{
			ECSEntityContainer* entityContainer = &m_Entities[entity - 1];

			if (!entityContainer->checkedForCompatibleSystems)
			{
				for (u32 i = 0; i < m_Systems.Size(); i++)
					CheckSystemEntityCompatibility(entityContainer, i);

				entityContainer->checkedForCompatibleSystems = true;
			}

			if (!entityContainer->enabled)
				return;

			for (u32 i = 0; i < entityContainer->compatibleSystems.Size(); i++)
			{
				SystemIndex index = entityContainer->compatibleSystems[i];
				ECSSystemContainer* system = &m_Systems[index];

				if (!system->actualSystem->enabled)
					continue;

				if (processType == EU_ECS_SYSTEM_PROCESS_UPDATE)
					system->actualSystem->ProcessEntityOnUpdate(entity, dt);
				else if (processType == EU_ECS_SYSTEM_PROCESS_RENDER)
					system->actualSystem->ProcessEntityOnRender(entity);
			}

			for (u32 i = 0; i < entityContainer->children.Size(); i++)
				ProcessEntity(entityContainer->children[i], processType, dt);
		}

		inline void CheckSystemEntityCompatibility(ECSEntityContainer* entity, SystemIndex system)
		{
			ECSSystemContainer* systemContainer = &m_Systems[system];
			b32 hasAllRequiredComponents = true;
			for (u32 i = 0; i < systemContainer->actualSystem->m_NumRequiredComponents; i++)
			{
				b32 foundRequiredSystemComponentInEntity = false;
				for(u32 j = 0; j < entity->components.Size(); j++)
				{
					if(systemContainer->actualSystem->m_RequiredComponets[i] == entity->components[j].typeID)
					{
						foundRequiredSystemComponentInEntity = true;
						break;
					}
				}

				if(!foundRequiredSystemComponentInEntity)
				{
					hasAllRequiredComponents = false;
					break;
				}
			}

			if (hasAllRequiredComponents)
			{
				b32 systemAlreadyInList = false;
				for (u32 i = 0; i < entity->compatibleSystems.Size(); i++)
				{
					if (entity->compatibleSystems[i] == system)
					{
						systemAlreadyInList = true;
						break;
					}
				}

				if (!systemAlreadyInList)
					entity->compatibleSystems.Push(system);
			}
		}
	private:
		List<ECSEntityContainer> m_Entities;
		List<ECSSystemContainer> m_Systems;
		List<u32> m_FreeEntityIDs;
		Map<metadata_typeid, DynamicPoolAllocator*> m_ComponentAllocators;
		DynamicPoolAllocator m_SystemAllocator;
	};

}