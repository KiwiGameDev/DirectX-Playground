#include "EditorApplication.h"
#include "GraphicsEngine.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Rigidbody.h"
#include "CapsuleCollider.h"
#include "BoxCollider.h"
#include <fstream>

EditorApplication* Singleton<EditorApplication>::instance = nullptr;

void EditorApplication::saveScene(const std::string& file_path)
{
	std::ofstream outfile;
	outfile.open(file_path);
	
	for (auto name_gameobject_pair : GameObjectManager::get().getGameObjectMap())
	{
		std::string name = name_gameobject_pair.first;
		GameObject* gameobject = name_gameobject_pair.second;

		outfile << "GameObject " << name << '\n';
		
		if (gameobject->hasComponent<Transform>())
		{
			const Transform& transform = gameobject->getComponent<Transform>();
			const Vector3 pos = transform.getPosition();
			const Vector3 rot = transform.getOrientationEuler();
			const Vector3 scale = transform.getScale();

			outfile << "Component Transform" << '\n';
			outfile << "Position " << pos.x << " " << pos.y << " " << pos.z << '\n';
			outfile << "Rotation " << rot.x << " " << rot.y << " " << rot.z << '\n';
			outfile << "Scale " << scale.x << " " << scale.y << " " << scale.z << '\n';
		}

		if (gameobject->hasComponent<MeshRenderer>())
		{
			outfile << "Component MeshRenderer" << '\n';
			outfile << "Mesh " << gameobject->getComponent<MeshRenderer>().getMeshName() << '\n';
			outfile << "Texture " << gameobject->getComponent<MeshRenderer>().getTextureName() << '\n';
		}

		if (gameobject->hasComponent<Rigidbody>())
		{
			reactphysics3d::BodyType type = gameobject->getComponent<Rigidbody>().getBodyType();
			outfile << "Component Rigidbody" << '\n';
			outfile << "Type " << (type == reactphysics3d::BodyType::DYNAMIC ? "dynamic" : "static") << '\n';
		}

		if (gameobject->hasComponent<BoxCollider>())
		{
			outfile << "Component BoxCollider" << '\n';
		}

		if (gameobject->hasComponent<CapsuleCollider>())
		{
			outfile << "Component CapsuleCollider" << '\n';
		}

		outfile << "END\n";
	}

	outfile.close();
}

void EditorApplication::loadScene(const std::string& file_path)
{
	std::ifstream infile;
	infile.open(file_path);
	
	GameObjectManager::get().clearAllGameObjects();

	std::string input;

	while (infile >> input)
	{
		if (input == "GameObject")
		{
			infile >> input;
			GameObject* gameobject = new GameObject(input);
			
			while (infile >> input)
			{
				if (input == "Component")
				{
					std::string component_name;
					infile >> component_name;
					
					if (component_name == "Transform")
					{
						Vector3 p;
						Vector3 r;
						Vector3 s;

						infile >> input;
						if (input == "Position")
						{
							infile >> p.x >> p.y >> p.z;
						}
						else
						{
							throw new std::exception("Invalid level format!");
						}
						
						infile >> input;
						if (input == "Rotation")
						{
							infile >> r.x >> r.y >> r.z;
						}
						else
						{
							throw new std::exception("Invalid level format!");
						}
						
						infile >> input;
						if (input == "Scale")
						{
							infile >> s.x >> s.y >> s.z;
						}
						else
						{
							throw new std::exception("Invalid level format!");
						}

						gameobject->addComponent<Transform>(gameobject);
						gameobject->getComponent<Transform>().setPosition(p);
						gameobject->getComponent<Transform>().setOrientationEuler(r);
						gameobject->getComponent<Transform>().setScale(s);
					}
					else if (component_name == "MeshRenderer")
					{
						std::string mesh_name;
						std::string texture_name;

						infile >> input;
						if (input == "Mesh")
						{
							infile >> mesh_name;
						}
						else
						{
							throw new std::exception("Invalid level file!");
						}

						infile >> input;
						if (input == "Texture")
						{
							infile >> texture_name;
						}
						else
						{
							throw new std::exception("Invalid level file!");
						}

						ConstantBufferPtr cb = GraphicsEngine::get().getConstantBuffer();
						MeshPtr mesh = GraphicsEngine::get().getMeshManager().getMeshFromFile(mesh_name);
						TexturePtr tex = GraphicsEngine::get().getTextureManager().getTextureFromFile(texture_name);
						VertexShaderPtr vs = nullptr;
						PixelShaderPtr ps = nullptr;

						if (tex == nullptr)
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
						gameobject->getComponent<MeshRenderer>().setTexture(tex);
					}
					else if (component_name == "Rigidbody")
					{
						std::string type;

						infile >> input;
						if (input == "Type")
						{
							infile >> type;
						}
						else
						{
							throw new std::exception("Invalid level format!");
						}
						
						reactphysics3d::BodyType body_type = input == "dynamic"
							? reactphysics3d::BodyType::DYNAMIC
							: reactphysics3d::BodyType::STATIC;

						gameobject->addComponent<Rigidbody>(body_type, gameobject);
					}
					else if (component_name == "BoxCollider")
					{
						Vector3 scale = gameobject->getComponent<Transform>().getScale();
						gameobject->addComponent<BoxCollider>(scale * 0.5f, gameobject);
					}
					else if (component_name == "CapsuleCollider")
					{
						gameobject->addComponent<CapsuleCollider>(0.5f, 1.0f, gameobject);
					}
				}
				else if (input == "END")
				{
					break;
				}
			}

			GameObjectManager::get().addGameObject(gameobject);
		}
	}
	
	infile.close();
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
