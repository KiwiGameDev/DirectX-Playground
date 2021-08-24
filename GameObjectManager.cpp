#include "GameObjectManager.h"
#include "GameObject.h"

GameObjectManager* Singleton<GameObjectManager>::instance = nullptr;

void GameObjectManager::update()
{
	for (auto nameGameObjectPair : name_to_gameobject_map)
	{
		GameObject* gameObject = nameGameObjectPair.second;
		gameObject->update();
	}
}

void GameObjectManager::draw()
{
	for (auto nameGameObjectPair : name_to_gameobject_map)
	{
		GameObject* gameObject = nameGameObjectPair.second;
		gameObject->draw();
	}
}

GameObject* GameObjectManager::getGameObject(const std::string& name) const
{
	return name_to_gameobject_map.at(name);
}

void GameObjectManager::addGameObject(GameObject* gameobject)
{
	name_to_gameobject_map.insert({ gameobject->getName(), gameobject });
}

void GameObjectManager::removeGameObject(const std::string& name)
{
	name_to_gameobject_map.erase(name_to_gameobject_map.find(name));
}
