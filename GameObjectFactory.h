#pragma once
#include <string>

class GameObject;

class GameObjectFactory
{
public:
	static GameObject* createEmptyGameObject(const std::string& name);
	static GameObject* createTeapot(const std::string& name);
	static GameObject* createStaticPhysicsPlane(const std::string& name);
	static GameObject* createPhysicsCube(const std::string& name);
};
