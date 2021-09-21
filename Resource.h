#pragma once
#include <string>

class Resource
{
public:
	Resource(const std::string& name);

	std::string getName() const;

	virtual ~Resource() = default;

private:
	std::string m_name;
};
