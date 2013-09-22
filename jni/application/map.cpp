#include <map.h>

Map::Map(Zeni::String level) {

	LoadMap(level);
}

Map::~Map() {

	ClearMap();
}

void Map::LoadMap(Zeni::String level) {

	Zeni::String filepath = "/assets/maps/map"+level+".txt";
	std::ifstream infile;
	infile.open(filepath.c_str());
	
	if(!infile.is_open())
		assert(5);
	
	//Map is going to be 13x8 tiles
	for(int i = 0; i < 8; i++) {
		for(int j = 0; j < 13; j++) {
			char tile_type;
			infile >> tile_type;
			int tile_number = std::atoi(&tile_type);
			if(tile_number > 0) { //0 means no tile located there
				Tile *new_tile = new Tile();
				new_tile->position = Zeni::Point2f(854/13 * i, 480/8 * j);
				new_tile->size = Zeni::Vector2f(854/13, 480/8);
				tiles.push_back(new_tile);
			}
		}
	}


}

void Map::ClearMap() {
	for(int i = 0; i < tiles.size(); i++)
	{
		delete tiles[i];
	}
	tiles.clear();
}

void Map::RenderMap(Zeni::Video &video) {

}

Tile* Map::GetTile(Zeni::Point2f pos) {
	return new Tile();
}