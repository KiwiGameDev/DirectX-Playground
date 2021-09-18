#include "MeshManager.h"
#include "Mesh.h"

MeshPtr MeshManager::getMeshFromFile(const std::string& file_path)
{
	return std::static_pointer_cast<Mesh>(getResourceFromFile(file_path));
}

Resource* MeshManager::createResourceFromFileConcrete(const std::string& file_path)
{
	Mesh* mesh = nullptr;

	try
	{
		mesh = new Mesh(file_path);
	}
	catch (...)
	{

	}

	return mesh;
}
