#pragma once
#include "zenilib.h"
#include "rect.h"
#include "orb_container.h"

class Player {
private:
	
	float speed, jump_speed, gravity;
	Zeni::Point2f position;
	Zeni::Vector2f size;
	Zeni::Point2f velocity;
	OrbContainer orbcontainer;
	
public:
	bool move_left, move_right, jump, activate_gravity, release_orb;
	Rect *current, *previous;
	Zeni::Point2f tile_id;
	
	Player();
	~Player();
	
	void Render();
	void Move(float time_step);
	void Update(float time_step);
	void Collision(Orb &orb);

	Zeni::Point2f GetPosition();
	Zeni::Point2f GetVelocity();
	Zeni::Vector2f GetSize();

	void SetPosition(Zeni::Point2f pos);
	void SetPosition(float x_, float y_);
	void SetVelocity(Zeni::Point2f vel);
	void SetVelocity(float x_, float y_);
	void SetSize(Zeni::Vector2f size);


};