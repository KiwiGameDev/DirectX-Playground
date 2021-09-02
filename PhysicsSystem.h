#pragma once
#include <reactphysics3d/reactphysics3d.h>

class BoxPhysicsComponent;

class PhysicsSystem
{
public:
	PhysicsSystem();

	void registerComponent(BoxPhysicsComponent* component);
	void unregisterComponent(BoxPhysicsComponent* component);
	void unregisterComponentByName(const std::string& name);

	void update();
	
	reactphysics3d::PhysicsCommon* getPhysicsCommon() const;
	reactphysics3d::PhysicsWorld* getPhysicsWorld() const;

	BoxPhysicsComponent* getComponentByName(const std::string& name) const;
	const std::vector<BoxPhysicsComponent*>& getAllComponents() const;

	~PhysicsSystem();

private:
	reactphysics3d::PhysicsCommon* m_physics_common;
	reactphysics3d::PhysicsWorld* m_physics_world;

	std::vector<BoxPhysicsComponent*> m_component_list;
	std::unordered_map<std::string, BoxPhysicsComponent*> m_component_map;
};
