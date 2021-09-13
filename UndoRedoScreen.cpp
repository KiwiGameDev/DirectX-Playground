#include "UndoRedoScreen.h"

#include "GameObjectManager.h"
#include "imgui.h"
#include "imgui_internal.h"

UndoRedoScreen::UndoRedoScreen(const std::string& name)
	: Screen(name)
{
	
}

void UndoRedoScreen::draw()
{
	ImGui::Begin("Undo Redo");

	bool isUndoButtonDisabled = GameObjectManager::get().getUndoCommandsCount() == 0;
	bool isRedoButtonDisabled = GameObjectManager::get().getRedoCommandsCount() == 0;

	// Undo Button
	if (isUndoButtonDisabled)
	{
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
	}
	if (ImGui::Button("Undo"))
	{
		GameObjectManager::get().undoLastCommand();
	}
	if (isUndoButtonDisabled)
	{
		ImGui::PopItemFlag();
		ImGui::PopStyleVar();
	}

	// Redo Button
	if (isRedoButtonDisabled)
	{
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
	}
	if (ImGui::Button("Redo"))
	{
		GameObjectManager::get().redoLastCommand();
	}
	if (isRedoButtonDisabled)
	{
		ImGui::PopItemFlag();
		ImGui::PopStyleVar();
	}
	
	ImGui::End();
}
