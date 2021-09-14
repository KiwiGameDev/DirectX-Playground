#include "PhysicsSystem.h"
#include "BoxPhysicsComponent.h"
#include "GameObject.h"
#include "Time.h"

PhysicsSystem::PhysicsSystem()
{
	m_physics_common = new reactphysics3d::PhysicsCommon();

	reactphysics3d::PhysicsWorld::WorldSettings world_settings;
	world_settings.defaultVelocitySolverNbIterations = 50;
	world_settings.gravity = reactphysics3d::Vector3(0, -9.81, 0);
	m_physics_world = m_physics_common->createPhysicsWorld(world_settings);
}

void PhysicsSystem::addComponent(BoxPhysicsComponent* physics_component)
{
	m_component_map.insert({ physics_component->getOwner()->getName(), physics_component });
}

void PhysicsSystem::removeComponent(BoxPhysicsComponent* physics_component)
{
	m_component_map.erase(physics_component->getOwner()->getName());
}

void PhysicsSystem::update()
{
	if (Time::get().deltaTime() <= 0.0f)
		return;

	for (auto name_component_pair : m_component_map)
	{
		name_component_pair.second->prePhysicsUpdate();
	}
	
	m_physics_world->update(Time::get().deltaTime());
	
	for (auto name_component_pair : m_component_map)
	{
		name_component_pair.second->postPhysicsUpdate();
	}
}

reactphysics3d::PhysicsCommon* PhysicsSystem::getPhysicsCommon() const
{
	return m_physics_common;
}

reactphysics3d::PhysicsWorld* PhysicsSystem::getPhysicsWorld() const
{
	return m_physics_world;
}

BoxPhysicsComponent* PhysicsSystem::getComponentByName(const std::string& name) const
{
	return m_component_map.at(name);
}

PhysicsSystem::~PhysicsSystem()
{
	delete m_physics_common;
}
