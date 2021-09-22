#pragma once
#include <string>

class GameObject;

class GameObjectFactory
{
public:
	static GameObject* createEmptyGameObject(const std::string& name);
	static GameObject* createTeapot(const std::string& name);
	static GameObject* createStaticPhysicsPlane(const std::string& name);
	static GameObject* createTexturedPhysicsCube(const std::string& name);
	static GameObject* createSphere(const std::string& name);
	static GameObject* createPhysicsSphere(const std::string& name);
	static GameObject* createCapsule(const std::string& name);
	static GameObject* createPhysicsCapsule(const std::string& name);
};
