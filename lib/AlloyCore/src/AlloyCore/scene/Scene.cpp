#include "AlloyCore/scene/Scene.hpp"

namespace Alloy::Internal
{

	/* -- PUBLIC -- */

	Scene::Scene()
		: m_ComponentRegistry()
		, m_RootEntityID(NullEntity)
	{
		m_RootEntityID = createEntityWithBaseComponents();
	}

	EntityID Scene::SpawnEntity()
	{
		return SpawnChildEntity(m_RootEntityID);
	}

	EntityID Scene::SpawnChildEntity(EntityID parentID)
	{
		EntityID id = createEntityWithBaseComponents();

		SetParent(id, parentID);

		return id;
	}

	void Scene::DespawnEntity(EntityID id)
	{
		m_ComponentRegistry.RemoveEntity(id);
	}

	void Scene::SetParent(EntityID childID, EntityID parentID)
	{
		auto& parentComponent = m_ComponentRegistry.GetComponent<Parent>(childID);

		// get old parent
		EntityID oldParentID = parentComponent.ParentID;

		// check if not the same as new parent
		if (oldParentID == parentID)
			return;

		// remove from old parent's children list
		if (oldParentID != NullEntity)
		{
			auto& children = m_ComponentRegistry.GetComponent<Children>(oldParentID).ChildrenIDs;
			std::erase(children, childID);
		}

		// set new parent
		parentComponent.ParentID = parentID;

		// add to new parent's children list
		m_ComponentRegistry.GetComponent<Children>(parentID).ChildrenIDs.push_back(childID);
	}


	/* -- PRIVATE -- */

	EntityID Scene::createEntityWithBaseComponents()
	{
		EntityID id = m_ComponentRegistry.CreateEntityID();

		// add hierarchy components
		m_ComponentRegistry.AddComponent<Parent>(id);
		m_ComponentRegistry.AddComponent<Children>(id);

		return id;
	}
}