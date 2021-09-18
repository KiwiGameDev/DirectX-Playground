#include "GameObjectManager.h"
#include "EditorApplication.h"
#include "GameObject.h"
#include "Transform.h"
#include "BoxPhysicsComponent.h"

GameObjectManager* Singleton<GameObjectManager>::instance = nullptr;

GameObjectManager::GameObjectManager()
{
	EditorApplication::get().addStateChangedEventListener(new EventCallback(this, &GameObjectManager::onEditorStateChanged));
}

void GameObjectManager::start()
{
	for (auto nameGameObjectPair : m_name_to_gameobject_map)
	{
		GameObject* gameObject = nameGameObjectPair.second;
		gameObject->start();
	}
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
}

void GameObjectManager::removeGameObject(const std::string& name)
{
	m_name_to_gameobject_map.erase(m_name_to_gameobject_map.find(name));
}

void GameObjectManager::clearAllGameObjects()
{
	for (auto nameGameObjectPair : m_name_to_gameobject_map)
	{
		delete nameGameObjectPair.second;
	}

	m_name_to_gameobject_map.clear();
}

void GameObjectManager::saveGameObjectsStartingState()
{
	for (auto name_gameobject_pair : m_name_to_gameobject_map)
	{
		std::string name = name_gameobject_pair.first;
		GameObject* gameobject = name_gameobject_pair.second;
		m_gameobject_starting_position.insert_or_assign(name, gameobject->getComponent<Transform>().getPosition());
		m_gameobject_starting_rotation.insert_or_assign(name, gameobject->getComponent<Transform>().getOrientation());
		m_gameobject_starting_scale.insert_or_assign(name, gameobject->getComponent<Transform>().getScale());
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
	m_commands_undo.push(command);
	
	while (!m_commands_redo.empty())
	{
		delete m_commands_redo.top();
		m_commands_redo.pop();
	}
}

void GameObjectManager::undoLastCommand()
{
	if (m_commands_undo.empty())
		return;
	
	ICommand* command = m_commands_undo.top();
	command->unexecute();
	
	m_commands_redo.push(command);
	m_commands_undo.pop();
}

void GameObjectManager::redoLastCommand()
{
	if (m_commands_redo.empty())
		return;
	
	ICommand* command = m_commands_redo.top();
	command->execute();

	m_commands_undo.push(command);
	m_commands_redo.pop();
}

void GameObjectManager::clearAllCommands()
{
	while (!m_commands_undo.empty())
	{
		delete m_commands_undo.top();
		m_commands_undo.pop();
	}

	while (!m_commands_redo.empty())
	{
		delete m_commands_redo.top();
		m_commands_redo.pop();
	}
}

uint16_t GameObjectManager::getUndoCommandsCount() const
{
	return m_commands_undo.size();
}

uint16_t GameObjectManager::getRedoCommandsCount() const
{
	return m_commands_redo.size();
}

void GameObjectManager::loadGameObjectsStartingState()
{
	for (auto name_gameobject_pair : m_name_to_gameobject_map)
	{
		std::string name = name_gameobject_pair.first;
		GameObject* gameobject = name_gameobject_pair.second;
		gameobject->getComponent<Transform>().setPosition(m_gameobject_starting_position[name]);
		gameobject->getComponent<Transform>().setOrientation(m_gameobject_starting_rotation[name]);
		gameobject->getComponent<Transform>().setScale(m_gameobject_starting_scale[name]);

		// Physics workaround :)
		if (gameobject->hasComponent<BoxPhysicsComponent>())
		{
			gameobject->getComponent<BoxPhysicsComponent>().setAdjusted(true);
		}
	}
}
