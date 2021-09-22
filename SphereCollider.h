#pragma once
#include "Component.h"

class SphereCollider : public Component
{
public:
	SphereCollider(float radius, GameObject* owner);

	void perform() override;

protected:
	void onStart() override;

private:
	float m_radius;
};
