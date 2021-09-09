#pragma once
#include "Singleton.h"
#include "Matrix4x4.h"
#include <reactphysics3d/mathematics/Quaternion.h>
#include <unordered_map>

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

private:
	GameObjectManager();

	void loadGameObjectsStartingState();
	
	std::unordered_map<std::string, GameObject*> m_name_to_gameobject_map;
	std::unordered_map<std::string, Vector3> m_gameobject_starting_position;
	std::unordered_map<std::string, reactphysics3d::Quaternion> m_gameobject_starting_rotation;
	std::unordered_map<std::string, Vector3> m_gameobject_starting_scale;
};
