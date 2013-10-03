#pragma once
#include <zenilib.h>
#include "orb.h"
#include "player.h"
#include "map.h"
#include <vector>

class OrbLogic {

private:
	std::vector<Orb *> active_orbs;
	std::vector<Orb *> active_black_orbs;
	Zeni::Chronometer<Zeni::Time> logic_timer, difficulty_timer;
	Zeni::Random random;
	Zeni::Point2f GetStartPosition();
	int black_orbs;
public:
	OrbLogic();
	~OrbLogic();
	void Update(float time_step);
	void Render();
	void Collision(Player &player);
	void Collision(Map &map);
};