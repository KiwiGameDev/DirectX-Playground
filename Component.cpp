#include "Component.h"
#include "GameObject.h"

Component::Component(GameObject* owner)
	: m_owner(owner)
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

void Component::start()
{
	if (m_has_started)
		return;

	onStart();

	m_has_started = true;
}

GameObject* Component::getOwner() const
{
	return m_owner;
}
