#pragma once
#include "zenilib.h"
#include "rect.h"

class Player {
private:
	
	float speed, jump_speed, gravity;
	Zeni::Point2f position;
	Zeni::Vector2f size;
	Zeni::Point2f velocity;
	
public:
	bool move_left, move_right, jump, activate_gravity;
	Rect *current, *previous;
	
	Player();
	~Player();
	
	void Render();
	void Move(float time_step);
	void Update(float time_step);
	
	Zeni::Point2f GetPosition();
	Zeni::Point2f GetVelocity();
	Zeni::Vector2f GetSize();

	void SetPosition(Zeni::Point2f pos);
	void SetPosition(float x_, float y_);
	void SetVelocity(Zeni::Point2f vel);
	void SetVelocity(float x_, float y_);
	void SetSize(Zeni::Vector2f size);


};