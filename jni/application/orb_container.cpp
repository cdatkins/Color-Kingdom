#include "orb_container.h"

OrbContainer::OrbContainer(Zeni::Point2f position_) {
	
	position = position_;
}

OrbContainer::~OrbContainer() {
	
}

int OrbContainer::GetSize() {
	return orbs.size();
}

void OrbContainer::AddOrb(Orb * orb_) {
	orbs.push_back(orb_);
}

Orb::Color OrbContainer::GetFrontColor() {
	if(orbs.size() > 0) {
	   return orbs.front()->color;
	}
	else 
		return Orb::NONE;
}

void OrbContainer::ChangeAll(Orb::Color color_) {

	for(int i =0; i < orbs.size(); i++) {
		orbs[i]->color = color_;
	}
}

Orb* OrbContainer::ReleaseOrb() {
	
	if(orbs.size() > 0) {
		Orb * new_orb = orbs.front();
		orbs.pop_front();
		return new_orb;
	}
	else 
		return NULL;
}

void OrbContainer::ReleaseAll() {
	
	for(int i = 0; i < orbs.size(); i++) {
		ReleaseOrb();
	}
	orbs.clear();
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