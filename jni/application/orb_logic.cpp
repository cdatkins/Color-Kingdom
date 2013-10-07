#include "orb_logic.h"
#include "math.h"
#include <time.h>
#include <cstdlib>
#include <iostream>

OrbLogic::OrbLogic() {
	
	logic_timer.start();
	difficulty_timer.start();
	black_orbs = 3;
}

OrbLogic::~OrbLogic() {
	for(int i = 0; i < active_orbs.size(); i++) {
		delete active_orbs[i];
	}

	for(int i = 0; i < active_black_orbs.size(); i++) {
		delete active_black_orbs[i];
	}
	logic_timer.stop();
}

void OrbLogic::Render() {

	for(int i = 0; i < active_orbs.size(); i++) {
		active_orbs[i]->Render();
	}

	for(int i = 0; i < active_black_orbs.size(); i++) {
		active_black_orbs[i]->Render();
	}
}

Zeni::Point2f OrbLogic::GetStartPosition() {

	Zeni::Point2f position;
	switch(random.rand_lt(3)) {
	case 0: //Left side of the screen
		position.y = random.rand_lt(400);
		position.x = -20;
		break;
	case 1: //Top side of the screen
		position.y = -20;
		position.x = random.rand_lt(800)+20;
		break;
	case 2: //Right side of the screen
		position.y = random.rand_lt(400);
		position.x = 870;
		break;
	}

	return position;
}

void OrbLogic::Update(float time_step) {
	
	//Check for orbs to delete
	for(int i = 0; i < active_orbs.size(); i++) {
		
		if(!active_orbs[i]->active && active_orbs[i]->state == Orb::OFFSCREEN) {
			delete active_orbs[i];
			active_orbs.erase(active_orbs.begin() + i);
		}
	}

	for(int i = 0; i < active_black_orbs.size(); i++) {
		if(!active_black_orbs[i]->active && active_black_orbs[i]->state == Orb::OFFSCREEN) {
			
			delete active_black_orbs[i];
			active_black_orbs.erase(active_black_orbs.begin() + i);
		}
	}
	
	//Updated difficulty as time goes on
	if( difficulty_timer.seconds() >= 100) {
		black_orbs+=2;
		difficulty_timer.reset();
	}

	//Decides when to add new orbs every 10 seconds
	if(logic_timer.seconds() >= 10) {
		
		if(random.rand_lt(5) > 0 ) { //80% of the time send out reds
			active_orbs.push_back( new Orb(Orb::RED, GetStartPosition()));
			active_orbs.push_back(new Orb(Orb::RED, GetStartPosition()));
			active_orbs.push_back(new Orb(Orb::RED, GetStartPosition()));
		}
		if(random.rand_lt(2)) { //50% of the time send out greens
			active_orbs.push_back(new Orb(Orb::GREEN, GetStartPosition()));
			active_orbs.push_back(new Orb(Orb::GREEN, GetStartPosition()));
		}
		if(random.rand_lt(3) == 1) { //30% of the time send out blues
			active_orbs.push_back(new Orb(Orb::BLUE, GetStartPosition()));
		}

		if(active_black_orbs.size() < black_orbs) {
			
			int orbs_needed =  black_orbs - active_black_orbs.size();
			for(int i = 0; i <orbs_needed; i++) {
				active_black_orbs.push_back(new Orb(Orb::BLACK, GetStartPosition()));
			}
		}
		logic_timer.reset();
	}
	
	for(int i = 0; i < active_orbs.size(); i++) {
		active_orbs[i]->Update(time_step);
	}

	for(int i = 0; i < active_black_orbs.size(); i++) {
		active_black_orbs[i]->Update(time_step);
	}
}

void OrbLogic::Collision(Player& player) {
	
	for(int i = 0; i < active_orbs.size(); i++) {
		player.Collision(active_orbs[i]);
	}

	for(int i = 0; i < active_black_orbs.size(); i++) {
		player.Collision(active_black_orbs[i]);
	}
}

void OrbLogic::Collision(Map& map) {

	for(int i = 0; i < active_orbs.size(); i++) {
		
		map.Collision(*active_orbs[i]);
	}

	for(int i = 0; i < active_black_orbs.size(); i++) {
		map.Collision(*active_black_orbs[i]);
	}
}