#pragma once

class BlackHoleFacade
{
public:
	BlackHoleFacade() = default;
	virtual ~BlackHoleFacade() = default;
	virtual void create_holes(int n) = 0;
	virtual void remove_all_holes() = 0;
};
