#include <zenilib.h>
#include <fstream>
#include <vector>

struct Tile {
	//enum State { SOLID, PASSIVE };
	Zeni::Point2f position;
	Zeni::Vector2f size;
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
};