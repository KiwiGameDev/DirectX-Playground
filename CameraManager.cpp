#include "CameraManager.h"

CameraManager* Singleton<CameraManager>::instance = nullptr;

void CameraManager::setEditorCamera(Camera* editor_camera)
{
	m_editor_camera = editor_camera;
}

Camera* CameraManager::getEditorCamera() const
{
	return m_editor_camera;
}
