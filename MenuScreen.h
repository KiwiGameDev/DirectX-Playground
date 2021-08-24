#pragma once
#include "Screen.h"

class MenuScreen : public Screen
{
public:
	MenuScreen(const std::string& name);

	void draw() override;

private:
	bool isCreditsScreenShowing = false;
};
