#pragma once
#include "ResourceManager.h"

class MeshManager : public ResourceManager
{
public:
	MeshManager();

	MeshPtr getMeshFromFile(const wchar_t* file_path);

	~MeshManager();

protected:
	Resource* createResourceFromFileConcrete(const wchar_t* file_path) override;
};
