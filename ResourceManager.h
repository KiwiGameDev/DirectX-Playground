#pragma once
#include "Prerequisites.h"
#include <unordered_map>
#include <string>

class ResourceManager
{
public:
	virtual ~ResourceManager() = default;

protected:
	ResourcePtr getResourceFromFile(const std::string& file_path);
	
	virtual Resource* createResourceFromFileConcrete(const std::string& file_path) = 0;

private:
	std::unordered_map<std::string, ResourcePtr> m_map_resources;
};
