#pragma once
#include "Screen.h"

class ColorPickerScreen : public Screen
{
public:
	ColorPickerScreen(const std::string& name);

	void draw() override;

	void show();

private:
	bool m_is_showing = false;

	float m_color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	bool m_alpha = true;
	bool m_alpha_bar = true;
	bool m_side_preview = true;
	int m_picker_mode = 2;
	int m_display_mode = 2;
};
