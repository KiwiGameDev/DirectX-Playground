#include "GameObjectManager.h"

#include <iostream>

#include "BoxPhysicsComponent.h"
#include "EditorApplication.h"
#include "GameObject.h"

GameObjectManager* Singleton<GameObjectManager>::instance = nullptr;

GameObjectManager::GameObjectManager()
{
	EditorApplication::get().addStateChangedEventListener(new EventCallback(this, &GameObjectManager::onEditorStateChanged));
}

void GameObjectManager::update()
{
	for (auto nameGameObjectPair : m_name_to_gameobject_map)
	{
		GameObject* gameObject = nameGameObjectPair.second;
		gameObject->update();
	}
}

void GameObjectManager::draw()
{
	for (auto nameGameObjectPair : m_name_to_gameobject_map)
	{
		GameObject* gameObject = nameGameObjectPair.second;
		gameObject->draw();
	}
}

GameObject* GameObjectManager::getGameObject(const std::string& name) const
{
	return m_name_to_gameobject_map.at(name);
}

const std::unordered_map<std::string, GameObject*>& GameObjectManager::getGameObjectMap() const
{
	return m_name_to_gameobject_map;
}

void GameObjectManager::addGameObject(GameObject* gameobject)
{
	m_name_to_gameobject_map.insert({ gameobject->getName(), gameobject });
	gameobject->awake();
}

void GameObjectManager::removeGameObject(const std::string& name)
{
	m_name_to_gameobject_map.erase(m_name_to_gameobject_map.find(name));
}

void GameObjectManager::saveGameObjectsStartingState()
{
	for (auto name_gameobject_pair : m_name_to_gameobject_map)
	{
		std::string name = name_gameobject_pair.first;
		GameObject* gameobject = name_gameobject_pair.second;
		m_gameobject_starting_position.insert_or_assign(name, gameobject->getPosition());
		m_gameobject_starting_rotation.insert_or_assign(name, gameobject->getOrientation());
		m_gameobject_starting_scale.insert_or_assign(name, gameobject->getScale());
	}
}

void GameObjectManager::onEditorStateChanged()
{
	if (EditorApplication::get().getState() == EditorApplication::State::Play || EditorApplication::get().getState() == EditorApplication::State::Step)
	{
		clearAllCommands();
	}
	
	if (EditorApplication::get().getState() == EditorApplication::State::Stop)
	{
		loadGameObjectsStartingState();
	}
}

void GameObjectManager::addAndExecuteCommand(ICommand* command)
{
	command->execute();
	m_command_stack.push(command);
	std::cout << "Commands size " << m_command_stack.size() << '\n';
}

void GameObjectManager::removeandUnexecuteLastCommand()
{
	if (m_command_stack.empty())
		return;

	std::cout << "Commands size " << m_command_stack.size() << '\n';
	
	ICommand* command = m_command_stack.top();
	command->unexecute();
	delete command;
	
	m_command_stack.pop();
}

void GameObjectManager::clearAllCommands()
{
	while (!m_command_stack.empty())
	{
		m_command_stack.pop();
	}
}

void GameObjectManager::loadGameObjectsStartingState()
{
	for (auto name_gameobject_pair : m_name_to_gameobject_map)
	{
		std::string name = name_gameobject_pair.first;
		GameObject* gameobject = name_gameobject_pair.second;
		gameobject->setPosition(m_gameobject_starting_position[name]);
		gameobject->setOrientation(m_gameobject_starting_rotation[name]);
		gameobject->setScale(m_gameobject_starting_scale[name]);

		// Physics workaround :)
		Component* component = gameobject->getComponentByType(Component::Type::Physics);
		if (BoxPhysicsComponent* physics_component = dynamic_cast<BoxPhysicsComponent*>(component))
		{
			physics_component->setAdjusted(true);
		}
	}
}
