#include "ComponentSystem.h"

ComponentSystem* Singleton<ComponentSystem>::instance = nullptr;

PhysicsSystem& ComponentSystem::getPhysicsSystem()
{
	return m_physics_system;
}
