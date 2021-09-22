#pragma once
#include <reactphysics3d/reactphysics3d.h>

class Rigidbody;

class PhysicsSystem
{
public:
	PhysicsSystem();

	void addComponent(Rigidbody* physics_component);
	void removeComponent(Rigidbody* physics_component);

	void update();
	
	reactphysics3d::PhysicsCommon* getPhysicsCommon() const;
	reactphysics3d::PhysicsWorld* getPhysicsWorld() const;

	Rigidbody* getComponentByName(const std::string& name) const;

	~PhysicsSystem();

private:
	reactphysics3d::PhysicsCommon* m_physics_common;
	reactphysics3d::PhysicsWorld* m_physics_world;
	
	std::unordered_map<std::string, Rigidbody*> m_component_map;
};
