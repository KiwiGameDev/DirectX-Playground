#include "MoveGameObjectCommand.h"
#include "GameObject.h"
#include "Transform.h"

MoveGameObjectCommand::MoveGameObjectCommand(GameObject* gameobject, Vector3 new_position)
	: m_gameobject(gameobject), m_old_position(gameobject->getComponent<Transform>().getPosition()), m_new_position(new_position)
{
	
}

void MoveGameObjectCommand::execute()
{
	m_gameobject->getComponent<Transform>().setPosition(m_new_position);
}

void MoveGameObjectCommand::unexecute()
{
	m_gameobject->getComponent<Transform>().setPosition(m_old_position);
}

void MoveGameObjectCommand::updateNewPosition(Vector3 new_position)
{
	m_new_position = new_position;
}
