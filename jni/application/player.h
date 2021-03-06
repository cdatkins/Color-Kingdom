#pragma once
#include "zenilib.h"
#include "rect.h"
#include "orb_container.h"
#include "item.h"

class Player {
private:
	
	
	float speed, jump_speed, gravity;
	Zeni::Point2f position;
	Zeni::Point2f respawn_position;
	Zeni::Vector2f size;
	Zeni::Point2f velocity;
	OrbContainer orbcontainer;
	Zeni::Chronometer<Zeni::Time> respawn_timer;
	
public:
	bool move_left, move_right, jump, activate_gravity, release_orb;
	Rect *current, *previous;
	Zeni::Point2f tile_id;
	Orb::Color tile_color;
	int num_lives;
	int score;
	int multiplier[3];
	bool active;
	enum State {STAND, LEFT, RIGHT, JUMPL, JUMPR};
	State state;
	Item *curr_item;
	Player();
	~Player();
	
	void Render();
	void Move(float time_step);
	void Update(float time_step);
	void Collision(Orb *orb);
	void Collision(Item *item);
	void Respawn();
	
	Zeni::Point2f GetPosition();
	Zeni::Point2f GetVelocity();
	Zeni::Vector2f GetSize();

	void SetPosition(Zeni::Point2f pos);
	void SetPosition(float x_, float y_);
	void SetVelocity(Zeni::Point2f vel);
	void SetVelocity(float x_, float y_);
	void SetSize(Zeni::Vector2f size);


};