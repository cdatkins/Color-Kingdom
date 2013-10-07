#include "player.h"

Player::Player()
	:position(427.0f,240.0f), velocity(0.0f,0.0f), size(32.0f,64.0f), speed(250.0f),
	jump_speed(550.0f), move_left(false), move_right(false), activate_gravity(true),
	gravity(700.0f), jump(false), orbcontainer(position), num_lives(3), release_orb(false),
	active(true), state(STAND), score(0), curr_item(NULL)
{
	multiplier[0] = 1;
	multiplier[1] = 1;
	multiplier[2] = 1;
	respawn_position = position;
	current = new Rect(position.x,position.y,size.i,size.j);
	previous = new Rect(position.x,position.y,size.i,size.j);
}

Player::~Player() {
	delete current;
	delete previous;
}

void Player::Render() {
	
	if(active) {
		switch (state) {
		case STAND :
			Zeni::render_image("player_stand", position, position + size);
			break;
		case LEFT :
			Zeni::render_image("player", position, position + size, true);
			break;
		case RIGHT :
			Zeni::render_image("player", position, position + size);
			break;
		case JUMPR :
			Zeni::render_image("player_jump", position, position + size);
			break;
		case JUMPL :
			Zeni::render_image("player_jump", position, position + size, true);
			break;
		}
		orbcontainer.Render();
		
	}
}

void Player::Move(float time_step) {
	
	if(jump && !activate_gravity) {
		velocity.y = -jump_speed;
		activate_gravity = true;
		Zeni::play_sound("jump",1.0f,.25f);
	}
	else if(!jump) { //allow player to short jump or jump higher when when button is held
		if(velocity.y  < -225.0f) 
			velocity.y = -225.0f;
	}

	if(move_left) {
		velocity.x = -speed;
		if(!activate_gravity)
			state = LEFT;
		else
			state = JUMPL;
	}
	else if(move_right) {
		velocity.x = speed;
		if(!activate_gravity)
			state = RIGHT;
		else
			state = JUMPR;
	}
	else if(!activate_gravity) {
		velocity.x = 0;
		state = STAND;
	}

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
	
	if(active) {
		previous->SetPosition(current->GetX(), current->GetY());
		Move(time_step);
		orbcontainer.Update(position);
		
		/*
		if(curr_item != NULL) {	
			
			if(curr_item->done) {
				//curr_item->state = Item::COLLECTABLE;
				//curr_item-> active = false;
				if(curr_item->type == Item::MULTI) {
					multiplier[0] = 1;
					multiplier[1] = 1;
					multiplier[2] = 1;
				}
				curr_item = NULL;
			}
		}*/

		
		//Releasing the Orb into the buckets
		if(release_orb && tile_color == orbcontainer.GetFrontColor()
			&& !activate_gravity) {
			
			Orb* orb = orbcontainer.ReleaseOrb();
			orb->active = false;
			orb->state = Orb::OFFSCREEN;
			
			switch(orb->color) {
			case Orb::BLUE:
				score += 50*multiplier[2];
				break;
			case Orb::GREEN:
				score += 30*multiplier[1];
				break;
			case Orb::RED:
				score += 10*multiplier[0];
				break;
			}
			release_orb = false;
			Zeni::play_sound("score");
		}
		else {
			release_orb = false;
		}
		
		//Stopping timer when player is colliable with orbs again after respawning
		if(respawn_timer.is_running() && respawn_timer.seconds() >= 3) { 
			respawn_timer.stop();
			respawn_timer.reset();
		}
		current->SetPosition(position.x,position.y);
	}
	else {
		Respawn();
	}
}

void Player::Respawn() {
	
	if(respawn_timer.seconds() >= 2  && num_lives > 0) {
		position = respawn_position;
		orbcontainer.Update(position);
		current->SetPosition(position.x,position.y);
		previous->SetPosition(current->GetX(), current->GetY());
		velocity = Zeni::Point2f(0.0f,0.0f);
		state = STAND;
		activate_gravity = true;
		active = true;
		respawn_timer.reset();
	}
}

void Player::Collision(Orb * orb) {
	if(active) {
		if(orb->active && current->Intersects(*orb->current)) {
			if(orb->color != Orb::BLACK && orbcontainer.GetSize() < 5) {
				orb->state = Orb::CAPTURED;
				orbcontainer.AddOrb(orb);
			}
			else if(orb->color == Orb::BLACK && !respawn_timer.is_running()) {
				num_lives --;
				active = false;
				orbcontainer.ReleaseAll();
				respawn_timer.start();
			}
		}
	}
}

void Player::Collision(Item *item) {
	if(active && !respawn_timer.is_running()) {
		
		if(item->active && item->state == Item::COLLECTABLE) {
			
			if(current->Intersects(*item->current)) {
				curr_item = item;
				item->life_time.stop();
				item->state = Item::CAPUTRED;
				
				if(item->type == Item::MULTI) {
					item->bonus_time.start();
					switch(item->color) {
					case Item::BLUE:
						multiplier[2] = 2;
						break;
					case Item::GREEN:
						multiplier[1] = 2;
						break;
					case Item::RED:
						multiplier[0] = 2;
						break;
					}
				}
				else if(item->type == Item::CHANGE) {
					switch(item->color) {
					case Item::BLUE:
						orbcontainer.ChangeAll(Orb::BLUE);
						break;
					case Item::GREEN:
						orbcontainer.ChangeAll(Orb::GREEN);
						break;
					case Item::RED:
						orbcontainer.ChangeAll(Orb::RED);
						break;
					}
				}
			}
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