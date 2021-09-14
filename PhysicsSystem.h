#pragma once
#include <reactphysics3d/reactphysics3d.h>

class BoxPhysicsComponent;

class PhysicsSystem
{
public:
	PhysicsSystem();

	void addComponent(BoxPhysicsComponent* physics_component);
	void removeComponent(BoxPhysicsComponent* physics_component);

	void update();
	
	reactphysics3d::PhysicsCommon* getPhysicsCommon() const;
	reactphysics3d::PhysicsWorld* getPhysicsWorld() const;

	BoxPhysicsComponent* getComponentByName(const std::string& name) const;

	~PhysicsSystem();

private:
	reactphysics3d::PhysicsCommon* m_physics_common;
	reactphysics3d::PhysicsWorld* m_physics_world;
	
	std::unordered_map<std::string, BoxPhysicsComponent*> m_component_map;
};
