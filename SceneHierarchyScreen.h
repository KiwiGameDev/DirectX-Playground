#pragma once
#include "Screen.h"

class InspectorScreen;

class SceneHierarchyScreen : public Screen
{
public:
	SceneHierarchyScreen(InspectorScreen* inspector_screen, const std::string& name);

	void draw() override;

private:
	InspectorScreen* m_inspector_screen;
};

