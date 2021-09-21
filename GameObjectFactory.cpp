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
	MeshPtr mesh = GraphicsEngine::get().getMeshManager().getMeshFromFile("Assets/Meshes/teapot.obj");
	VertexShaderPtr vs = GraphicsEngine::get().getVertexShaderManager().getVertexShaderFromFile("TexturedVertexShader.hlsl");
	PixelShaderPtr ps = GraphicsEngine::get().getPixelShaderManager().getPixelShaderFromFile("TexturedPixelShader.hlsl");
	ConstantBufferPtr cb = GraphicsEngine::get().getConstantBuffer();
	TexturePtr tex = GraphicsEngine::get().getTextureManager().getTextureFromFile("Assets/Textures/brick.png");

	GameObject* teapot = new GameObject(name);
	teapot->addComponent<Transform>(teapot);
	teapot->getComponent<Transform>().setPosition(Vector3(0.0f, 1.0f, 1.0f));
	teapot->addComponent<MeshRenderer>(teapot, mesh, cb, vs, ps);
	teapot->getComponent<MeshRenderer>().setTexture(tex);
	return teapot;
}

GameObject* GameObjectFactory::createStaticPhysicsPlane(const std::string& name)
{
	MeshPtr mesh = GraphicsEngine::get().getMeshManager().getMeshFromFile("Assets/Meshes/cube.obj");
	VertexShaderPtr vs = GraphicsEngine::get().getVertexShaderManager().getVertexShaderFromFile("ColoredVertexShader.hlsl");
	PixelShaderPtr ps = GraphicsEngine::get().getPixelShaderManager().getPixelShaderFromFile("ColoredPixelShader.hlsl");
	ConstantBufferPtr cb = GraphicsEngine::get().getConstantBuffer();

	GameObject* plane = new GameObject(name);
	plane->addComponent<Transform>(plane);
	plane->getComponent<Transform>().setScale(Vector3(32.0f, 0.1f, 32.0f));
	plane->getComponent<Transform>().setPosition(Vector3(0.0f, 0.0f, 0.0f));
	plane->addComponent<MeshRenderer>(plane, mesh, cb, vs, ps);
	plane->addComponent<BoxPhysicsComponent>(plane->getComponent<Transform>().getScale() * 0.5f, reactphysics3d::BodyType::STATIC, plane);
	return plane;
}

GameObject* GameObjectFactory::createTexturedPhysicsCube(const std::string& name)
{
	MeshPtr mesh = GraphicsEngine::get().getMeshManager().getMeshFromFile("cube_uv.obj");
	VertexShaderPtr vs = GraphicsEngine::get().getVertexShaderManager().getVertexShaderFromFile("TexturedVertexShader.hlsl");
	PixelShaderPtr ps = GraphicsEngine::get().getPixelShaderManager().getPixelShaderFromFile("TexturedPixelShader.hlsl");
	ConstantBufferPtr cb = GraphicsEngine::get().getConstantBuffer();
	TexturePtr tex = GraphicsEngine::get().getTextureManager().getTextureFromFile("Assets/Textures/brick.png");

	GameObject* cube = new GameObject(name);
	cube->addComponent<Transform>(cube);
	cube->addComponent<MeshRenderer>(cube, mesh, cb, vs, ps);
	cube->getComponent<MeshRenderer>().setTexture(tex);
	cube->addComponent<BoxPhysicsComponent>(cube->getComponent<Transform>().getScale() * 0.5f, reactphysics3d::BodyType::DYNAMIC, cube);
	return cube;
}

GameObject* GameObjectFactory::createSphere(const std::string& name)
{
	MeshPtr mesh = GraphicsEngine::get().getMeshManager().getMeshFromFile("sphere.obj");
	VertexShaderPtr vs = GraphicsEngine::get().getVertexShaderManager().getVertexShaderFromFile("ColoredVertexShader.hlsl");
	PixelShaderPtr ps = GraphicsEngine::get().getPixelShaderManager().getPixelShaderFromFile("ColoredPixelShader.hlsl");
	ConstantBufferPtr cb = GraphicsEngine::get().getConstantBuffer();

	GameObject* cube = new GameObject(name);
	cube->addComponent<Transform>(cube);
	cube->addComponent<MeshRenderer>(cube, mesh, cb, vs, ps);
	return cube;
}

GameObject* GameObjectFactory::createCapsule(const std::string& name)
{
	MeshPtr mesh = GraphicsEngine::get().getMeshManager().getMeshFromFile("capsule.obj");
	VertexShaderPtr vs = GraphicsEngine::get().getVertexShaderManager().getVertexShaderFromFile("ColoredVertexShader.hlsl");
	PixelShaderPtr ps = GraphicsEngine::get().getPixelShaderManager().getPixelShaderFromFile("ColoredPixelShader.hlsl");
	ConstantBufferPtr cb = GraphicsEngine::get().getConstantBuffer();

	GameObject* cube = new GameObject(name);
	cube->addComponent<Transform>(cube);
	cube->addComponent<MeshRenderer>(cube, mesh, cb, vs, ps);
	return cube;
}
