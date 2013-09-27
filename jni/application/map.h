#pragma once
#include <zenilib.h>
#include <fstream>
#include <vector>
#include "player.h"
#include "orb.h"

struct Tile {
	//enum State { SOLID, PASSIVE };
	Zeni::Point2f position;
	Zeni::Point2f id;
	Zeni::Vector2f size;
	Zeni::String tile_type;
	Rect *rect;
	bool contain_entity;
	//State state;
};

class Map {
private:
	std::vector<Tile*> tiles;
public:
	Map(Zeni::String level);
	~Map();
	void LoadMap(Zeni::String level); //Only load in solid tiles
	void ClearMap();
	void RenderMap(Zeni::Video &video);
	Tile* GetTile(Zeni::Point2f pos); //Returns a single from the map
	void Collision(Player& player);
	void Collision(Orb & orb);
};