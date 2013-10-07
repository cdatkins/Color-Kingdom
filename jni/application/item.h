#pragma once
#include "zenilib.h"
#include "rect.h"

class Item {

private:
	Zeni::Point2f position;
	Zeni::Point2f velocity;
	float speed;
	float gravity;
	Zeni::Vector2f size;
	Zeni::Point2f GetStartPosition();
	Zeni::Random random;
	
public:
	
	Zeni::Chronometer<Zeni::Time> life_time, bonus_time;
	enum Type { MULTI, CHANGE };
	enum ItemColor { RED, GREEN, BLUE};
	enum ItemState { COLLECTABLE, CAPUTRED };

	Type type;
	ItemColor color;
	ItemState state;
	Rect *current, *previous;
	bool active, activate_gravity,done;

	Item(ItemColor c, Type t);
	~Item();
	void Update(float time_step);
	void Render();

};

