#pragma once
#include "Component.h"
#include "Vector3.h"

class BoxCollider : public Component
{
public:
	BoxCollider(const Vector3& half_extents, GameObject* owner);
	
	void perform() override;

protected:
	void onStart() override;
	
private:
	Vector3 m_half_extents;
};
