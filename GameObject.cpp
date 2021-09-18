#include "GameObject.h"
#include "GraphicsEngine.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include <utility>

GameObject::GameObject(const std::string& name)
	: name(name)
{
	addComponent<Transform>(this);
}

void GameObject::start()
{
	if (!m_is_enabled)
		return;
	
	for (auto name_component_pair : m_component_map)
	{
		name_component_pair.second->start();
	}
}

void GameObject::update()
{
	if (!m_is_enabled)
		return;

	for (auto name_component_pair : m_component_map)
	{
		name_component_pair.second->perform();
	}
}

void GameObject::draw()
{
	if (!m_is_enabled)
		return;

	if (hasComponent<MeshRenderer>())
	{
		getComponent<MeshRenderer>().draw();
	}
}

std::string GameObject::getName() const
{
	return name;
}

bool GameObject::getIsEnabled() const
{
	return m_is_enabled;
}

GameObject::~GameObject()
{
	for (auto name_component_pair : m_component_map)
	{
		delete name_component_pair.second;
	}
}
