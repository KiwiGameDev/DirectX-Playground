#include "PhysicsSystem.h"
#include "BoxPhysicsComponent.h"
#include "Time.h"

PhysicsSystem::PhysicsSystem()
{
	m_physics_common = new reactphysics3d::PhysicsCommon();

	reactphysics3d::PhysicsWorld::WorldSettings world_settings;
	world_settings.defaultVelocitySolverNbIterations = 50;
	world_settings.gravity = reactphysics3d::Vector3(0, -9.81, 0);
	m_physics_world = m_physics_common->createPhysicsWorld(world_settings);
}

void PhysicsSystem::registerComponent(BoxPhysicsComponent* component)
{
	if (m_component_map.find(component->getName()) == m_component_map.end())
	{
		m_component_list.push_back(component);
		m_component_map.insert({ component->getName(), component });
	}
	else
	{
		std::cerr << component->getName() << " is already attached!\n";
	}
}

void PhysicsSystem::unregisterComponent(BoxPhysicsComponent* component)
{
	if (m_component_map.find(component->getName()) != m_component_map.end())
	{
		m_component_list.erase(std::ranges::find(m_component_list, component));
		m_component_map.erase(component->getName());
	}
	else
	{
		std::cerr << component->getName() << " is already detached!\n";
	}
}

void PhysicsSystem::unregisterComponentByName(const std::string& name)
{
	auto itr = m_component_map.find(name);
	
	if (itr != m_component_map.end())
	{
		m_component_list.erase(std::ranges::find(m_component_list, itr->second));
		m_component_map.erase(name);
	}
	else
	{
		std::cerr << name << " is already detached!\n";
	}
}

void PhysicsSystem::update()
{
	if (Time::get().deltaTime() <= 0.0f)
		return;

	for (BoxPhysicsComponent* component : m_component_list)
	{
		component->prePerform();
	}
	
	m_physics_world->update(Time::get().deltaTime());
	
	for (BoxPhysicsComponent* component : m_component_list)
	{
		component->perform();
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

const std::vector<BoxPhysicsComponent*>& PhysicsSystem::getAllComponents() const
{
	return m_component_list;
}

PhysicsSystem::~PhysicsSystem()
{
	delete m_physics_common;
}
