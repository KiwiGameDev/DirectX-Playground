#pragma once
#include "Screen.h"

class ViewportScreen : public Screen
{
public:
	ViewportScreen(const std::string& name);
	
	void draw() override;
};

