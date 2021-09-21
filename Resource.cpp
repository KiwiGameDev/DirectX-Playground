#include "Resource.h"

Resource::Resource(const std::string& name)
{
	size_t name_index_start = name.find_last_of('\\');

	if (name_index_start != std::string::npos)
	{
		m_name = name.substr(name_index_start + 1, name.size() - name_index_start - 1);
	}
	else
	{
		m_name = name;
	}
}

std::string Resource::getName() const
{
	return m_name;
}
