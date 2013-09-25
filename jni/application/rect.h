#pragma once
class Rect
{
private:
	float x, y, w, h, top, bottom, left, right;
public:
	enum Side {TOP,BOTTOM,LEFT,RIGHT};
	Rect(float x_, float y_, float w_, float h_);

	float GetX();
	float GetY();
	float GetW();
	float GetH();

	float GetTop();
	float GetBottom();
	float GetLeft();
	float GetRight();

	void SetPosition(float x_, float y_);
	void SetDeminsion(float w_, float h_);

	bool Intersects(Rect box);
};