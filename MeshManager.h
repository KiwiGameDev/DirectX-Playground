#pragma once
#include "ResourceManager.h"

class MeshManager : public ResourceManager
{
public:
	MeshPtr getMeshFromFile(const std::string& file_path);

protected:
	Resource* createResourceFromFileConcrete(const std::string& file_path) override;
};
