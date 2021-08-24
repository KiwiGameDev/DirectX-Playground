#pragma once
#include <string>

class Screen
{
public:
	Screen(const std::string& name);

	virtual void draw() = 0;

	std::string getName() const;

	virtual ~Screen();

private:
	std::string name;
};
