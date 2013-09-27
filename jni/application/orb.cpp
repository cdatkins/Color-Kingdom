#include "orb.h"
#include <time.h>
#include <stdlib.h>

Orb::Orb(Color color_, Zeni::Point2f pos_) 
	: velocity(0.0f,0.0f), size(16.0f,16.0f), state(START)
{
	color = color_;
	position = pos_;
	current = new Rect(position.x,position.y,size.i,size.j);
	previous = new Rect(position.x,position.y,size.i,size.j);
	
	srand(time(NULL));
	
	switch (color) {
		case RED:
			stop_time = 10.0f;
			break;
		case GREEN:
			stop_time = 7.0f;
			break;
		case BLUE:
			stop_time = 5.0f;
			break;
		case BLACK:
			stop_time = 15.0f;
			break;
	}

	if(position.x < 0 && position.y > 0 && position.y < 480) {  //leftside
		
		int num = rand() % 2;
		if(num == 0)
			direction = Direction::UPRIGHT;
		else
			direction = Direction::DOWNRIGHT;
	}
	else if(position.x > 854 && position.y > 0 && position.y < 480) {  //rightside
		int num = rand() % 2;
		if(num == 0)
			direction = Direction::UPLEFT;
		else
			direction = Direction::DOWNLEFT;
	}
	else if(position.x > 0 && position.x < 854 && position.y < 0) {
		int num = rand() % 2;
		if(num == 0)
			direction = Direction::DOWNLEFT;
		else
			direction = Direction::DOWNRIGHT;
	}
	
}

Orb::Orb(Color color_) 
	:color(color_), state(CAPTURED)
{}

Orb::~Orb() {
	
	if(color == GREEN || color == RED || color == BLUE || color == BLACK) {
		delete current;
		delete previous;
	}

}

void Orb::Move(float time_step) {
	
	switch(direction)
	{
		case UPLEFT:
			velocity.x = -100.0f;
			velocity.y = -100.0f;
			break;
		case UPRIGHT:
			velocity.x = 100.0f;
			velocity.y = -100.0f;
			break;
		case DOWNLEFT:
			velocity.x = -100.0f;
			velocity.y = 100.0f;
			break;
		case DOWNRIGHT:
			velocity.x = 100.0f;
			velocity.y = 100.0f;
			break;
	}

	position.x += velocity.x * time_step;
	position.y += velocity.y * time_step; // + 150*sin(position.x * 2 * 3.1458/180)) * time_step;

	if(state == ONSCREEN) {
		
		if(position.x <= 0) {
			OnCollision(Rect::RIGHT);	
		}
		else if(position.x >= 854 - size.i) {
			OnCollision(Rect::LEFT);
		}
		else if(position.y <= 0) {
			OnCollision(Rect::BOTTOM);
		}
		else if(position.y >= 480 - size.j) {
			OnCollision(Rect::TOP);
		}

	}
}

void Orb::Update(float time_step) {

	previous->SetPosition(current->GetX(), current->GetY());
	
	Move(time_step);
	
	current->SetPosition(position.x,position.y);

	if(state == START && position.x > 0 - size.i && position.x < 854 + size.i 
		&& position.y > 0) { //Setup the orb for onscreen interactions
			state = ONSCREEN; 
			screen_time.start(); 
			active = true;
	}

	if(state == ONSCREEN && screen_time.seconds() >= stop_time) { //Tells the orb to go off screen
		
		state = OFFSCREEN;
	}

	if(state == CAPTURED || (state == OFFSCREEN && (position.x < 0 || position.x > 854
		|| position.y < 0))) { // sets the flag for rendering
			active = false;
	}
	
}

void Orb::OnCollision(Rect::Side s) {
	
	if(s == Rect::BOTTOM)
	{
		switch(direction)
		{
			case UPLEFT:
				direction = DOWNLEFT;
				break;
			case UPRIGHT:
				direction = DOWNRIGHT;
				break;
		}
	}
	else if(s == Rect::TOP)
	{
		switch(direction)
		{
			case DOWNLEFT:
				direction = UPLEFT;
				break;
			case DOWNRIGHT:
				direction = UPRIGHT;
				break;
		}
	}
	else if(s == Rect::LEFT)
	{
		switch(direction)
		{
			case UPRIGHT:
				direction = UPLEFT;
				break;
			case DOWNRIGHT:
				direction = DOWNLEFT;
				break;
		}
	}
	else if(s == Rect::RIGHT)
	{
		switch(direction)
		{
			case UPLEFT:
				direction = UPRIGHT;
				break;
			case DOWNLEFT:
				direction = DOWNRIGHT;
				break;
		}
	}
}

void Orb::Render() {
	
	if(active) {
		
		Zeni::render_image(ColorToString(color),position,position+size);
	}
}

Zeni::String Orb::ColorToString(Color color) {

	Zeni::String color_name = "none";
	switch (color) {
	case RED:
		color_name = "red";
		break;
	case BLUE:
		color_name = "blue";
		break;
	case GREEN:
		color_name = "green";
		break;
	case YELLOW:
		color_name = "yellow";
		break;
	case CYAN:
		color_name = "cyan";
		break;
	case PURPLE:
		color_name = "purple";
		break;
	case BLACK:
		color_name = "black";
		break;
	}
	return color_name;
}

Zeni::Point2f Orb::GetPosition() {
	return position;
}

Zeni::Point2f Orb::GetVelocity() {
	return velocity;
}

Zeni::Vector2f Orb::GetSize() {
	return size;
}

void Orb::SetPosition(Zeni::Point2f pos) {
	position = pos;
}

void Orb::SetPosition(float x_, float y_) {
	position = Zeni::Point2f(x_,y_);
}

void Orb::SetVelocity(Zeni::Point2f vel) {
	velocity = vel;
}

void Orb::SetVelocity(float x_, float y_) {
	velocity = Zeni::Point2f(x_,y_);
}

void Orb::SetSize(Zeni::Vector2f size_) {
	size = size_;
}