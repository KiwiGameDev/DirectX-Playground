#pragma once
#include "Singleton.h"
#include "Camera.h"

class CameraManager : public Singleton<CameraManager>
{
	friend class Singleton<CameraManager>;

public:
	void setEditorCamera(Camera* editor_camera);

	Camera* getEditorCamera() const;

private:
	Camera* m_editor_camera;
};
