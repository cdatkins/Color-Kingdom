#include "player.h"

Player::Player()
	:position(427.0f,240.0f), velocity(0.0f,0.0f), size(32.0f,32.0f), speed(250.0f),
	jump_speed(550.0f), move_left(false), move_right(false), activate_gravity(true),
	gravity(700.0f), jump(false), orbcontainer(position), num_lives(3)
{
	current = new Rect(position.x,position.y,size.i,size.j);
	previous = new Rect(position.x,position.y,size.i,size.j);
}

Player::~Player() {
	delete current;
	delete previous;
}

void Player::Render() {
	Zeni::render_image("player",position, position + size);
	orbcontainer.Render();
}

void Player::Move(float time_step) {
	
	if(jump && !activate_gravity) {

		velocity.y = -jump_speed;
		activate_gravity = true;
	}
	else if(!jump) {
		if(velocity.y  < -225.0f) 
			velocity.y = -225.0f;
	}

	if(move_left) {
		velocity.x = -speed;
	}
	else if(move_right) {
		velocity.x = speed;
	}
	else if(!activate_gravity)
		velocity.x = 0;

	if(activate_gravity) {
		velocity.y += gravity * time_step; 
	}
	else
		velocity.y = 0;

	position.x += velocity.x * time_step;
	position.y += velocity.y * time_step;
	
	if(position.x <= 0)
		position.x = 0;
	if(position.x >= 854 - size.i)
		position.x = 854 - size.i;
	if(position.y <= 0)
		position.y = 0;
}

void Player::Update(float time_step) {
	previous->SetPosition(current->GetX(), current->GetY());
	Move(time_step);
	orbcontainer.Update(position);
	if(release_orb && tile_color == orbcontainer.GetFrontColor()
		&& !activate_gravity) {
		orbcontainer.ReleaseOrb();
		release_orb = false;
	}
	current->SetPosition(position.x,position.y);
}

void Player::Collision(Orb * orb) {

	if(orb->active && current->Intersects(*orb->current)) {
		if(orb->color != Orb::BLACK && orbcontainer.GetSize() <= 5) {
			orb->state = Orb::CAPTURED;
			orbcontainer.AddOrb(orb);
		}
		else if(orb->color == Orb::BLACK) {
			//deduct a player life and reset the game
		}
	}
}

Zeni::Point2f Player::GetPosition() {
	return position;
}

Zeni::Point2f Player::GetVelocity() {
	return velocity;
}

Zeni::Vector2f Player::GetSize() {
	return size;
}

void Player::SetPosition(Zeni::Point2f pos) {
	position = pos;
}

void Player::SetPosition(float x_, float y_) {
	position = Zeni::Point2f(x_,y_);
}

void Player::SetVelocity(Zeni::Point2f vel) {
	velocity = vel;
}

void Player::SetVelocity(float x_, float y_) {
	velocity = Zeni::Point2f(x_, y_);
}
void Player::SetSize(Zeni::Vector2f size_) {
	size = size_;
}