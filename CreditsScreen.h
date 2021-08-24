#pragma once
#include "Prerequisites.h"
#include "Screen.h"

class CreditsScreen : public Screen
{
public:
	CreditsScreen(const std::string& name);

	void draw() override;
	
	void show();
	void hide();

private:
	TexturePtr m_logo;
	
	bool m_is_showing = false;
};

