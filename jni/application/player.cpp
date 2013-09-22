#include "player.h"

Player::Player()
	:position(0.0f,0.0f), speed(200.0f), size(16.0f,16.0f), move_left(false), move_right(false)
{}

void Player::Render() {
	Zeni::render_image("player",position, position + size);
}

void Player::Move(float time_step) {

	if(move_left)
	{
		position.x -= speed * time_step;
	}
	else if(move_right)
	{
		position.x += speed * time_step;
	}
}
