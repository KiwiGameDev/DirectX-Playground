#include "Component.h"
#include "GameObject.h"

Component::Component(const std::string& name, GameObject* owner, Type type)
	: m_name(name), m_owner(owner), m_type(type)
{
	
}

void Component::attachOwner(GameObject* owner)
{
	m_owner = owner;
}

void Component::detachOwner()
{
	m_owner = nullptr;
}

std::string Component::getName() const
{
	if (getOwner() != nullptr)
	{
		return m_owner->getName() + "_" + m_name;
	}
	else
	{
		return m_name;
	}
}

GameObject* Component::getOwner() const
{
	return m_owner;
}

Component::Type Component::getType() const
{
	return m_type;
}
