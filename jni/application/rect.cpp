#include "rect.h"

Rect::Rect(float x_, float y_, float w_, float h_) 
		:x(x_),y(y_),w(w_),h(h_)
{
	top = y;
	bottom = y+h;
	left = x;
	right = x+w;
}

float Rect::GetX() { return x;}
float Rect::GetY() { return y;}
float Rect::GetW() { return w;}
float Rect::GetH() { return h;}

float Rect::GetTop() { return top;}
float Rect::GetBottom() { return bottom;}
float Rect::GetLeft() { return left;}
float Rect::GetRight() { return right;}

void Rect::SetPosition(float x_, float y_) 
{

	x = x_; y = y_;
	
	top = y;
	bottom = y+h;
	left = x;
	right = x+w;
}
void Rect::SetDeminsion(float w_, float h_) { w = w_; h = h_;}

bool Rect::Intersects(Rect box)
{
	if(right < box.GetLeft() || left > box.GetRight() || 
		top > box.GetBottom() || bottom < box.GetTop())
		return false;
	else
		return true;
}