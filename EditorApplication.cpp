#include "EditorApplication.h"
#include "GraphicsEngine.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "BoxPhysicsComponent.h"
#include "MeshRenderer.h"
#include <json.hpp>
#include <fstream>


EditorApplication* Singleton<EditorApplication>::instance = nullptr;

void EditorApplication::saveScene(const std::string& file_path)
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
			components_json.push_back(
				{
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
				}
			);
		}

		if (gameobject->hasComponent<BoxPhysicsComponent>())
		{
			reactphysics3d::BodyType type = gameobject->getComponent<BoxPhysicsComponent>().getBodyType();
			
			components_json.push_back(
				{
					{ "name", "BoxPhysicsComponent" },
					{ "type", type == reactphysics3d::BodyType::DYNAMIC ? "dynamic" : "static" }
				}
			);
		}

		if (gameobject->hasComponent<MeshRenderer>())
		{
			components_json.push_back(
				{
					{ "name", "MeshRenderer" },
					{ "mesh", gameobject->getComponent<MeshRenderer>().getMeshName() },
					{ "texture", gameobject->getComponent<MeshRenderer>().getTextureName() }
				}
			);
		}

		out_json[name] = components_json;
	}

	std::ofstream outfile;
	outfile.open(file_path);
	outfile << out_json;
	outfile.close();
}

void EditorApplication::loadScene(const std::string& file_path)
{
	using json = nlohmann::json;
	
	json j;
	std::ifstream infile;
	infile.open(file_path);
	infile >> j;
	infile.close();

	GameObjectManager::get().clearAllGameObjects();

	for (const auto& jGameObject : j.items())
	{
		std::string gameobject_name = jGameObject.key();
		GameObject* gameobject = new GameObject(gameobject_name);

		for (const auto& component : jGameObject.value())
		{
			std::string component_name = component["name"];

			if (component_name == "Transform")
			{
				auto jPos = component["position"];
				auto jRot = component["rotation"];
				auto jScale = component["scale"];

				Vector3 pos(jPos[0], jPos[1], jPos[2]);
				Vector3 rot(jRot[0], jRot[1], jRot[2]);
				Vector3 scale(jScale[0], jScale[1], jScale[2]);

				gameobject->addComponent<Transform>(gameobject);
				gameobject->getComponent<Transform>().setPosition(pos);
				gameobject->getComponent<Transform>().setOrientationEuler(rot);
				gameobject->getComponent<Transform>().setScale(scale);
			}
			else if (component_name == "BoxPhysicsComponent")
			{
				Vector3 scale = gameobject->addComponent<Transform>(gameobject).getScale();
				reactphysics3d::BodyType body_type = component["type"] == "dynamic"
					? reactphysics3d::BodyType::DYNAMIC
					: reactphysics3d::BodyType::STATIC;
				
				gameobject->addComponent<BoxPhysicsComponent>(scale * 0.5f, body_type, gameobject);
			}
			else if (component_name == "MeshRenderer")
			{
				std::string mesh_name = component["mesh"];
				std::string texture_name = component["texture"];
				ConstantBufferPtr cb = GraphicsEngine::get().getConstantBuffer();
				MeshPtr mesh = GraphicsEngine::get().getMeshManager().getMeshFromFile(mesh_name);
				VertexShaderPtr vs = nullptr;
				PixelShaderPtr ps = nullptr;

				if (texture_name == "")
				{
					vs = GraphicsEngine::get().getVertexShaderManager().getVertexShaderFromFile("ColoredVertexShader.hlsl");
					ps = GraphicsEngine::get().getPixelShaderManager().getPixelShaderFromFile("ColoredPixelShader.hlsl");
				}
				else
				{
					vs = GraphicsEngine::get().getVertexShaderManager().getVertexShaderFromFile("TexturedVertexShader.hlsl");
					ps = GraphicsEngine::get().getPixelShaderManager().getPixelShaderFromFile("TexturedPixelShader.hlsl");
				}
				
				gameobject->addComponent<MeshRenderer>(gameobject, mesh, cb, vs, ps);
			}
		}

		GameObjectManager::get().addGameObject(gameobject);
	}
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
