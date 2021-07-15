#pragma once
#include "Prerequisites.h"
#include <unordered_map>
#include <string>

class ResourceManager
{
public:
	ResourceManager();

	ResourcePtr createResourceFromFile(const wchar_t* file_path);

	virtual ~ResourceManager();

protected:
	virtual Resource* createResourceFromFileConcrete(const wchar_t* file_path) = 0;

private:
	std::unordered_map<std::wstring, ResourcePtr> m_map_resources;
};
