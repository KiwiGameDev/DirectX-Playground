#pragma once
#include "PhysicsSystem.h"
#include "Singleton.h"

class ComponentSystem : public Singleton<ComponentSystem>
{
	friend class Singleton<ComponentSystem>;

public:
	PhysicsSystem& getPhysicsSystem();

private:
	PhysicsSystem m_physics_system;
};

