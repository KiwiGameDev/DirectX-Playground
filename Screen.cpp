#include "Screen.h"

Screen::Screen(const std::string& name)
	: name(name)
{
	
}

std::string Screen::getName() const
{
	return name;
}

Screen::~Screen()
{
	
}
