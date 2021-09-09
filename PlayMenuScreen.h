#pragma once
#include "Screen.h"

class PlayMenuScreen : public Screen
{
public:
	PlayMenuScreen(const std::string& name);

	void draw() override;
};
