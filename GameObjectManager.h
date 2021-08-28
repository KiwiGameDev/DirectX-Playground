#pragma once
#include <unordered_map>
#include "Singleton.h"

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

private:
	std::unordered_map<std::string, GameObject*> name_to_gameobject_map;
};
