#include "EditorApplication.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include <json.hpp>
#include <fstream>

#include "BoxPhysicsComponent.h"

EditorApplication* Singleton<EditorApplication>::instance = nullptr;

void EditorApplication::saveScene()
{
	using json = nlohmann::json;
	
	json out_json;
	
	for (auto name_gameobject_pair : GameObjectManager::get().getGameObjectMap())
	{
		std::string name = name_gameobject_pair.first;
		GameObject* gameobject = name_gameobject_pair.second;
		json components_json;
		
		if (gameobject->hasComponent<Transform>())
		{
			const Transform& transform = gameobject->getComponent<Transform>();
			components_json.push_back
			({
				{ "name", "Transform" },
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

		if (gameobject->hasComponent<BoxPhysicsComponent>())
		{
			components_json.push_back({{ "name", "BoxPhysicsComponent" }});
		}

		out_json[name] = components_json;
	}

	std::ofstream outfile;
	outfile.open("C:/dev/saves/scene.level");
	outfile << out_json;
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
