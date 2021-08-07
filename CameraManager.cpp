#include "CameraManager.h"

CameraManager* Singleton<CameraManager>::instance = nullptr;

void CameraManager::setGameCamera(Camera* cam)
{
	m_game_camera = cam;
}

void CameraManager::setEditorCamera(Camera* cam)
{
	m_editor_camera = cam;
}

Camera* CameraManager::getGameCamera() const
{
	return m_game_camera;
}

Camera* CameraManager::getEditorCamera() const
{
	return m_editor_camera;
}
