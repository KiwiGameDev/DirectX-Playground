#pragma once
#include "Screen.h"

class UndoRedoScreen : public Screen
{
public:
	UndoRedoScreen(const std::string& name);

	void draw() override;
};
