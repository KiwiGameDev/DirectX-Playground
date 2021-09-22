#pragma once
#include "Component.h"

class CapsuleCollider : public Component
{
public:
	CapsuleCollider(float height, float radius, GameObject* owner);

	void perform() override;

protected:
	void onStart() override;

private:
	float m_height;
	float m_radius;
};

