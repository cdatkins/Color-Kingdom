#include "orb_container.h"

OrbContainer::OrbContainer(Zeni::Point2f position_) {
	
	position = position_;
}

OrbContainer::~OrbContainer() {
	
	if(orbs.size() > 0) {
		for(int i = 0; i < orbs.size(); i++) {
			delete orbs[i];
		}
	}
}

void OrbContainer::AddOrb(Orb * orb_) {
	
	orbs.push_back(orb_);
}

Orb* OrbContainer::ReleaseOrb() {
	
	Orb * new_orb = orbs.front();
	orbs.pop_front();
	return new_orb;
}

void OrbContainer::Update(Zeni::Point2f position_) {

	position = position_;

	for(int i = 0; i < 5; i++) {
		slots[i].x = position.x + 8;
		slots[i].y = position.y - i*8 - 10;
	}
	
	
}

void OrbContainer::Render() {

	for(int i = 0; i < orbs.size(); i++) {
		Zeni::String filename = orbs[i]->ColorToString(orbs[i]->color);
		Zeni::render_image(filename + "_slot", slots[i], slots[i] + Zeni::Vector2f(16.0f,8.0f));
	}
	
	for(int i = 0 ; i < 5 - orbs.size(); i++) {

		Zeni::render_image("clear_slot", slots[4 - i], slots[4 - i] + Zeni::Vector2f(16.0f,8.0f));
	}
}