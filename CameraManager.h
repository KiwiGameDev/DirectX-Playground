#pragma once
#include "Singleton.h"
#include "Camera.h"

class CameraManager : public Singleton<CameraManager>
{
	friend class Singleton<CameraManager>;

public:
	void setGameCamera(Camera* cam);
	void setEditorCamera(Camera* cam);

	Camera* getGameCamera() const;
	Camera* getEditorCamera() const;

private:
	Camera* m_game_camera;
	Camera* m_editor_camera;
};
