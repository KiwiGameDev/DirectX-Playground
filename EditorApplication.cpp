#include "EditorApplication.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include <json.hpp>
#include <fstream>

EditorApplication* Singleton<EditorApplication>::instance = nullptr;

void EditorApplication::saveScene()
{
	using json = nlohmann::json;
	
	auto name_to_gameobject_map = GameObjectManager::get().getGameObjectMap();
	
	json j;
	
	for (auto name_gameobject_pair : name_to_gameobject_map)
	{
		std::string name = name_gameobject_pair.first;
		GameObject* gameobject = name_gameobject_pair.second;
		Transform& transform = gameobject->getComponent<Transform>();
		
		j.push_back
		({
			{ "name", gameobject->getName() },
			{
				"position",
				{
					transform.getPosition().x,
					transform.getPosition().y,
					transform.getPosition().z
				}
			},
			{
				"rotation",
				{
					transform.getOrientationEuler().x,
					transform.getOrientationEuler().y,
					transform.getOrientationEuler().z
				}
			},
			{
				"scale",
				{
					transform.getScale().x,
					transform.getScale().y,
					transform.getScale().z
				}
			}
		});
	}

	std::ofstream outfile;
	outfile.open("Saves/scene.json");
	outfile << j;
	outfile.close();
}

void EditorApplication::loadScene(const std::string& file_path)
{
	
}

void EditorApplication::addStateChangedEventListener(IEventCallback* callback)
{
	m_state_changed.addListener(callback);
}

void EditorApplication::removeStateChangedEventListener(IEventCallback* callback)
{
	m_state_changed.removeListener(callback);
}

void EditorApplication::setState(State state)
{
	if (m_state == state)
		return;
	
	m_state = state;
	m_state_changed.fire();
}

EditorApplication::State EditorApplication::getState() const
{
	return m_state;
}
