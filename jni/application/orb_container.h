#pragma once
#include <deque>
#include "orb.h"

class OrbContainer {

private:
	std::deque<Orb *> orbs;
	Zeni::Point2f slots[5];
	Zeni::Point2f position;

public:
	OrbContainer(Zeni::Point2f position_);
	~OrbContainer();
	int GetSize();
	void AddOrb(Orb * orb_);
	void ChangeAll(Orb::Color color);
	Orb::Color GetFrontColor();
	Orb* ReleaseOrb();
	void ReleaseAll();
	void Update(Zeni::Point2f position_);
	void Render();

};