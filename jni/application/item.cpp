#include "item.h"

Item::Item(ItemColor c, Type t) 
	:speed(200.0f), gravity(500.0f), size(32.0f,32.0f), velocity(0,0), active(true), activate_gravity(true),
	done(false)
{
	color = c;
	type = t;
	state = COLLECTABLE;
	position = GetStartPosition();

	current = new Rect(position.x,position.y,size.i,size.j);
	previous = new Rect(position.x,position.y,size.i,size.j);
	life_time.start();
}

Item::~Item() {
	delete current;
	delete previous;
}

void Item::Update(float time_step) {

	if(active) {

		previous->SetPosition(current->GetX(),current->GetY());
		
		if(state == COLLECTABLE && life_time.seconds() >= 10) {
			active = false;
			life_time.stop();
			done = true;
		}
		
		if(state == CAPUTRED) {
			if(type == MULTI && bonus_time.seconds() >= 15) {
				done = true;
				bonus_time.stop();
			}
			else if(type == CHANGE) {
				done = true;
			}
		}

		if(activate_gravity) {
			velocity.y += gravity * time_step;
		}
		else
			velocity.y = 0;
		position.y += velocity.y*time_step;
		current->SetPosition(position.x,position.y);
	}
}

void Item::Render() {
	
	Zeni::String filename;

	switch(type) {
	case MULTI:
		switch(color) {
		case RED:
			filename = "x2red";
			break;
		case GREEN:
			filename = "x2green";
			break;
		case BLUE:
			filename = "x2blue";
			break;
		}
		break;
	case CHANGE:
		switch(color) {
		case RED:
			filename = "starred";
			break;
		case BLUE:
			filename = "starblue";
			break;
		case GREEN:
			filename = "stargreen";
			break;
		}
	}
	if(active && state == COLLECTABLE) {
		Zeni::render_image(filename, position, position+size);
	}
	else if(state == CAPUTRED && type == MULTI) {
		Zeni::render_image(filename, Zeni::Point2f(427.0f,0.0f), Zeni::Point2f(459.0f,32.0f));
	}

}

Zeni::Point2f Item::GetStartPosition() {

	Zeni::Point2f position;
	switch(random.rand_lt(2)) {
	case 0: //Left 
		position.y = -32;
		position.x = random.rand_lt(166);
		break;
	case 2: //Middle
		position.y = -32;
		position.x = random.rand_lt(457)+199;
		break;
	case 1: //Right
		position.y = -32;
		position.x = random.rand_lt(164)+658;
		break;
	}

	return position;
}