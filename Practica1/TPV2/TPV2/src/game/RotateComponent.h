#pragma once
#include "../ecs/Component.h"
class Transform;

class RotateComponent : public ecs::Component
{
	float rot_amount_;
	Transform* trans_;

public:
	__CMPID_DECL__(ecs::cmp::ROTATE)

	RotateComponent();
	RotateComponent(float);
	void initComponent() override;
	void update() override;
};
