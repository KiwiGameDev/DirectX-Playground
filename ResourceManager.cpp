#include "ResourceManager.h"
#include "Resource.h"
#include <filesystem>

void ResourceManager::addResource(ResourcePtr resource)
{
	auto it = m_map_resources.find(resource->getName());

	if (it != m_map_resources.end())
		return;
	
	m_map_resources.insert({ resource->getName(), resource });
}

ResourcePtr ResourceManager::getResourceFromFile(const std::string& file_path)
{
	std::string file_name;
	size_t file_name_index = file_path.find_last_of('\\');

	if (file_name_index != std::string::npos)
	{
		file_name = file_path.substr(file_name_index + 1);
	}
	else
	{
		file_name_index = file_path.find_last_of('/');
		file_name = file_path.substr(file_name_index + 1);
	}
	
	auto it = m_map_resources.find(file_name);

	if (it != m_map_resources.end())
		return it->second;
	
	std::string full_path = std::filesystem::absolute(file_path.c_str()).string();
	Resource* raw_res = this->createResourceFromFileConcrete(full_path);

	if (raw_res)
	{
		ResourcePtr res(raw_res);
		m_map_resources.insert({ file_name, res });
		return res;
	}

	return nullptr;
}
