#include "orb.h"

Orb::Orb(Color color_, Zeni::Point2f pos_) 
	: velocity(200.0f,0.0f), size(16.0f,16.0f), state(START)
{
	color = color_;
	position = pos_;
	current = new Rect(position.x,position.y,size.i,size.j);
	previous = new Rect(position.x,position.y,size.i,size.j);

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
	
	position.x += velocity.x * time_step;
	position.y += (velocity.y + 25*sin(position.x * 3.1458/180)) * time_step;

	if(state == ONSCREEN) {
		
		if(position.x <= 0 || position.x + size.i >= 854) {
			velocity.x = -(velocity.x);
		}

	}
}

void Orb::Update(float time_step) {

	previous->SetPosition(current->GetX(), current->GetY());
	
	Move(time_step);
	
	current->SetPosition(position.x,position.y);

	if(state == START && position.x > 0 && position.x < 854-size.i 
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