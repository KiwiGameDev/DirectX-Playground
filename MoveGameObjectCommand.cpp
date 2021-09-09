#include "MoveGameObjectCommand.h"
#include "GameObject.h"

MoveGameObjectCommand::MoveGameObjectCommand(GameObject* gameobject, Vector3 new_position)
	: m_gameobject(gameobject), m_old_position(gameobject->getPosition()), m_new_position(new_position)
{
	
}

void MoveGameObjectCommand::execute()
{
	m_gameobject->setPosition(m_new_position);
}

void MoveGameObjectCommand::unexecute()
{
	m_gameobject->setPosition(m_old_position);
}

void MoveGameObjectCommand::updateNewPosition(Vector3 new_position)
{
	m_new_position = new_position;
}
