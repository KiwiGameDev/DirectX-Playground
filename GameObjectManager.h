#pragma once
#include <stack>

#include "Singleton.h"
#include "Matrix4x4.h"
#include <reactphysics3d/mathematics/Quaternion.h>
#include <unordered_map>

#include "ICommand.h"

class GameObject;

class GameObjectManager : public Singleton<GameObjectManager>
{
	friend class Singleton<GameObjectManager>;

public:
	void update();
	void draw();
	
	GameObject* getGameObject(const std::string& name) const;
	const std::unordered_map<std::string, GameObject*>& getGameObjectMap() const;

	void addGameObject(GameObject* gameobject);
	void removeGameObject(const std::string& name);

	void saveGameObjectsStartingState();
	void onEditorStateChanged();

	void addAndExecuteCommand(ICommand* command);
	void undoLastCommand();
	void redoLastCommand();
	void clearAllCommands();

	uint16_t getUndoCommandsCount() const;
	uint16_t getRedoCommandsCount() const;

private:
	GameObjectManager();

	void loadGameObjectsStartingState();

	std::stack<ICommand*> m_commands_undo;
	std::stack<ICommand*> m_commands_redo;
	std::unordered_map<std::string, GameObject*> m_name_to_gameobject_map;
	std::unordered_map<std::string, Vector3> m_gameobject_starting_position;
	std::unordered_map<std::string, reactphysics3d::Quaternion> m_gameobject_starting_rotation;
	std::unordered_map<std::string, Vector3> m_gameobject_starting_scale;
};
