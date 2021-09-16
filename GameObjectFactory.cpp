#include "GameObjectFactory.h"
#include "GraphicsEngine.h"
#include "GameObject.h"
#include "Mesh.h"
#include "BoxPhysicsComponent.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Prerequisites.h"

GameObject* GameObjectFactory::createEmptyGameObject(const std::string& name)
{
	return new GameObject(name);
}

GameObject* GameObjectFactory::createTeapot(const std::string& name)
{
	MeshPtr mesh = GraphicsEngine::get().getMeshManager().getMeshFromFile(L"Assets/Meshes/teapot.obj");
	VertexShaderPtr vs = GraphicsEngine::get().getVertexShaderManager().getVertexShaderFromFile(L"TexturedVertexShader.hlsl");
	PixelShaderPtr ps = GraphicsEngine::get().getPixelShaderManager().getPixelShaderFromFile(L"TexturedPixelShader.hlsl");
	ConstantBufferPtr cb = GraphicsEngine::get().getConstantBuffer();
	TexturePtr tex = GraphicsEngine::get().getTextureManager().getTextureFromFile(L"Assets/Textures/brick.png");

	GameObject* teapot = new GameObject(name);
	teapot->addComponent<MeshRenderer>(teapot, mesh->getVertexBuffer(), mesh->getIndexBuffer(), cb, vs, ps);
	teapot->getComponent<MeshRenderer>().setTexture(tex);
	teapot->addComponent<Transform>(teapot);
	teapot->getComponent<Transform>().setPosition(Vector3(0.0f, 1.0f, 1.0f));
	return teapot;
}

GameObject* GameObjectFactory::createStaticPhysicsPlane(const std::string& name)
{
	MeshPtr cube_mesh = GraphicsEngine::get().getMeshManager().getMeshFromFile(L"Assets/Meshes/cube.obj");
	VertexShaderPtr vs = GraphicsEngine::get().getVertexShaderManager().getVertexShaderFromFile(L"ColoredVertexShader.hlsl");
	PixelShaderPtr ps = GraphicsEngine::get().getPixelShaderManager().getPixelShaderFromFile(L"ColoredPixelShader.hlsl");
	ConstantBufferPtr cb = GraphicsEngine::get().getConstantBuffer();

	GameObject* plane = new GameObject(name);
	plane->addComponent<MeshRenderer>(plane, cube_mesh->getVertexBuffer(), cube_mesh->getIndexBuffer(), cb, vs, ps);
	plane->addComponent<Transform>(plane);
	plane->getComponent<Transform>().setScale(Vector3(32.0f, 0.1f, 32.0f));
	plane->getComponent<Transform>().setPosition(Vector3(0.0f, 0.0f, 0.0f));
	plane->addComponent<BoxPhysicsComponent>(plane->getComponent<Transform>().getScale(), reactphysics3d::BodyType::STATIC, plane);
	return plane;
}

GameObject* GameObjectFactory::createPhysicsCube(const std::string& name)
{
	MeshPtr mesh = GraphicsEngine::get().getMeshManager().getMeshFromFile(L"Assets/Meshes/cube.obj");
	VertexShaderPtr vs = GraphicsEngine::get().getVertexShaderManager().getVertexShaderFromFile(L"TexturedVertexShader.hlsl");
	PixelShaderPtr ps = GraphicsEngine::get().getPixelShaderManager().getPixelShaderFromFile(L"TexturedPixelShader.hlsl");
	ConstantBufferPtr cb = GraphicsEngine::get().getConstantBuffer();
	TexturePtr tex = GraphicsEngine::get().getTextureManager().getTextureFromFile(L"Assets/Textures/brick.png");

	GameObject* cube = new GameObject(name);
	cube->addComponent<MeshRenderer>(cube, mesh->getVertexBuffer(), mesh->getIndexBuffer(), cb, vs, ps);
	cube->getComponent<MeshRenderer>().setTexture(tex);
	cube->addComponent<Transform>(cube);
	cube->addComponent<BoxPhysicsComponent>(cube->getComponent<Transform>().getScale(), reactphysics3d::BodyType::DYNAMIC, cube);
	return cube;
}