#pragma once
#include "Matrix4x4.h"

__declspec(align(16))
struct ConstantBufferData
{
	Matrix4x4 m_world;
	Matrix4x4 m_game_cam;
	Matrix4x4 m_editor_view;
	Matrix4x4 m_editor_proj;
	int m_is_project;
	float m_time;
};
