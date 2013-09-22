#include "zenilib.h"

class Player {
private:
	Zeni::Point2f position;
	Zeni::Vector2f size;
	float speed;
	
public:
	bool move_left, move_right;
	Player();
	void Render();
	void Move(float time_step);

};