#include "CreditsScreen.h"
#include "GraphicsEngine.h"
#include "TextureManager.h"
#include "imgui.h"
#include "Texture.h"

CreditsScreen::CreditsScreen(const std::string& name)
	: Screen(name)
{
	m_logo = GraphicsEngine::get().getTextureManager().getTextureFromFile(L"Assets/Textures/dlsu.png");
}

void CreditsScreen::draw()
{
	if (!m_is_showing)
		return;
	
	ImGui::Begin("Credits", &m_is_showing);
	ImGui::Image(m_logo->getShaderResourceView(), ImVec2(128.0f, 128.0f));
	ImGui::Dummy(ImVec2(0.0f, 16.0f));
	ImGui::Text("Scene Editor v0.6.9");
	ImGui::Text("By Bruce Anakin C. Miranda");
	ImGui::Dummy(ImVec2(0.0f, 16.0f));
	ImGui::Text("Acknowledgements:");
	ImGui::Text("PardCode");
	ImGui::Text("Sir Neil");
	ImGui::Text("Saint La Salle");
	ImGui::End();
}

void CreditsScreen::show()
{
	m_is_showing = true;
}
