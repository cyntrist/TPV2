// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../ecs/Component.h"

class InfoMsgs: public ecs::Component {
public:
	InfoMsgs();
	virtual ~InfoMsgs();
	void render() override;
};

