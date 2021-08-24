#include "ColorPickerScreen.h"
#include "imgui.h"

ColorPickerScreen::ColorPickerScreen(const std::string& name)
	: Screen(name)
{
	
}

void ColorPickerScreen::draw()
{
    if (!m_is_showing)
        return;
	
    ImGuiColorEditFlags flags;
    if (!m_alpha)           flags |= ImGuiColorEditFlags_NoAlpha;
    if (m_alpha_bar)        flags |= ImGuiColorEditFlags_AlphaBar;
    if (!m_side_preview)    flags |= ImGuiColorEditFlags_NoSidePreview;

	if (m_picker_mode == 1)         flags |= ImGuiColorEditFlags_PickerHueBar;
    else if (m_picker_mode == 2)    flags |= ImGuiColorEditFlags_PickerHueWheel;

	if (m_display_mode == 1)        flags |= ImGuiColorEditFlags_NoInputs;
    else if (m_display_mode == 2)   flags |= ImGuiColorEditFlags_DisplayRGB;
    else if (m_display_mode == 3)   flags |= ImGuiColorEditFlags_DisplayHSV;
    else if (m_display_mode == 4)   flags |= ImGuiColorEditFlags_DisplayHex;

    ImGui::Begin("Color Picker", &m_is_showing);
    ImGui::ColorPicker4("MyColor", m_color, flags, nullptr);
    ImGui::End();
}

void ColorPickerScreen::show()
{
    m_is_showing = true;
}
