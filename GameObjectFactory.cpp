#include "GameObjectFactory.h"
#include "BoxPhysicsComponent.h"
#include "GameObject.h"
#include "Mesh.h"
#include "GraphicsEngine.h"
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
	TexturePtr bricks = GraphicsEngine::get().getTextureManager().getTextureFromFile(L"Assets/Textures/brick.png");

	GameObject* teapot = new GameObject(name, mesh->getVertexBuffer(), mesh->getIndexBuffer(), cb, vs, ps);
	teapot->addComponent<Transform>(teapot);
	teapot->getComponent<Transform>().setPosition(Vector3(0.0f, 1.0f, 1.0f));
	teapot->setTexture(bricks);
	return teapot;
}

GameObject* GameObjectFactory::createStaticPhysicsPlane(const std::string& name)
{
	MeshPtr cube_mesh = GraphicsEngine::get().getMeshManager().getMeshFromFile(L"Assets/Meshes/cube.obj");
	VertexShaderPtr vs = GraphicsEngine::get().getVertexShaderManager().getVertexShaderFromFile(L"ColoredVertexShader.hlsl");
	PixelShaderPtr ps = GraphicsEngine::get().getPixelShaderManager().getPixelShaderFromFile(L"ColoredPixelShader.hlsl");
	ConstantBufferPtr cb = GraphicsEngine::get().getConstantBuffer();

	GameObject* plane = new GameObject(name, cube_mesh->getVertexBuffer(), cube_mesh->getIndexBuffer(), cb, vs, ps);
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
	TexturePtr bricks = GraphicsEngine::get().getTextureManager().getTextureFromFile(L"Assets/Textures/brick.png");

	GameObject* cube = new GameObject(name, mesh->getVertexBuffer(), mesh->getIndexBuffer(), cb, vs, ps);
	cube->addComponent<Transform>(cube);
	cube->addComponent<BoxPhysicsComponent>(cube->getComponent<Transform>().getScale(), reactphysics3d::BodyType::DYNAMIC, cube);
	cube->setTexture(bricks);
	return cube;
}
