#include "ECS.h"

#include "Systems\TransformHierarchy2DSystem.h"

namespace Eunoia {

	void ECS::AddRequiredSystems()
	{
		CreateSystem<TransformHierarchy2DSystem>();
	}

	inline void ECS::UpdateTransformHierarchySystems(r32 dt)
	{
		for (u32 i = 0; i < m_Entities.Size(); i++)
		{
			ECSEntityContainer* entity = &m_Entities[i];
			if (!entity->checkedForCompatibleSystems)
			{
				for (u32 j = 0; j < m_Systems.Size(); j++)
					CheckSystemEntityCompatibility(entity, j);

				entity->checkedForCompatibleSystems = true;
			}

			for (u32 j = 0; j < entity->compatibleSystems.Size(); j++)
			{
				if (entity->compatibleSystems[j] == 0)
					m_Systems[entity->compatibleSystems[j]].actualSystem->ProcessEntityOnUpdate(i + 1, dt);
			}
		}
	}

}
