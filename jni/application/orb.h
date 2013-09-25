#pragma once
#include "zenilib.h"
#include "rect.h"


class Orb {
public:
	Rect *current, *previous;
	enum Color { RED, BLUE, GREEN, PURPLE, YELLOW, CYAN, BLACK };
	enum State { START, ONSCREEN, OFFSCREEN, CAPTURED };
	Color color;
	State state;
	Orb(Color color_, Zeni::Point2f pos_);
	Orb(Color color_); // Used for the created orbs that the player makes
	~Orb();

	void Move(float time_step);
	void Update(float time_step);
	void Render();
	Zeni::String ColorToString(Color color);

	Zeni::Point2f GetPosition();
	Zeni::Point2f GetVelocity();
	Zeni::Vector2f GetSize();

	void SetPosition(Zeni::Point2f pos);
	void SetPosition(float x_, float y_);
	void SetVelocity(Zeni::Point2f vel);
	void SetVelocity(float x_, float y_);
	void SetSize(Zeni::Vector2f size);

private:
	Zeni::Point2f position;
	Zeni::Point2f velocity;
	Zeni::Vector2f size;
	Zeni::Chronometer<Zeni::Time> screen_time; //used for the life_time of the orb
	float stop_time; 
	bool active;
};
