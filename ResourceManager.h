#pragma once
#include "Prerequisites.h"
#include <unordered_map>
#include <string>

class ResourceManager
{
public:
	ResourceManager();

	virtual ~ResourceManager();

protected:
	ResourcePtr getResourceFromFile(const wchar_t* file_path);
	
	virtual Resource* createResourceFromFileConcrete(const wchar_t* file_path) = 0;

private:
	std::unordered_map<std::wstring, ResourcePtr> m_map_resources;
};
