#include <map.h>
#include <iostream>
#include <sstream>

Map::Map(Zeni::String level) {

	LoadMap(level);
}

Map::~Map() {

	ClearMap();
}

void Map::LoadMap(Zeni::String level) {

	Zeni::String filepath = "../assets/maps/map"+level+".txt";
	std::ifstream infile;
	infile.open(filepath.c_str());
	
	if(!infile.is_open())
		std::cout << "Didn't open File";

		
	
	//Map is going to be 13x8 tiles
	for(int i = 0; i < 8; i++) {
		for(int j = 0; j < 13; j++) {
			char tile_type;
			infile >> tile_type;
			int tile_number = std::atoi(&tile_type);
			if(tile_number > 0) { //0 means no tile located there
				Tile *new_tile = new Tile();
				new_tile->position = Zeni::Point2f((854/13) * j, (480/8) * i);
				//std::cout << "Position: " << new_tile->position.x << " " << new_tile->position.y << std::endl;
				new_tile->tile_type = tile_type;
				new_tile->size = Zeni::Vector2f(854/13, 480/8);
				new_tile->rect = new Rect(new_tile->position.x,new_tile->position.y,new_tile->size.i,new_tile->size.j);
				tiles.push_back(new_tile);
			}
		}
	}


}

void Map::ClearMap() {
	for(int i = 0; i < tiles.size(); i++)
	{
		delete tiles[i]->rect;
		delete tiles[i];
	}
	tiles.clear();
}

void Map::RenderMap(Zeni::Video &video) {
	//Testing with render_image but want to use the video and apply textures
	for(int i = 0; i < tiles.size(); i++) {
		Zeni::render_image("floor_tile"+ tiles[i]->tile_type, tiles[i]->position, 
			tiles[i]->position + tiles[i]->size);
	}
}

void Map::Collision(Player &player) {
	
	player.activate_gravity = true;
	for(int i = 0; i < tiles.size(); i++) {
		
		if(player.current->Intersects(*tiles[i]->rect)) {
			Rect tile_rect = *tiles[i]->rect;
			

			if(player.current->GetBottom() >= tile_rect.GetTop() 
				&& player.previous->GetBottom() <= tile_rect.GetTop()) { //Player intersected from the top of the tile
					player.SetPosition(player.GetPosition().x,tile_rect.GetTop() - player.GetSize().j);
					player.activate_gravity = false;
			}
			else if(player.current->GetTop() <= tile_rect.GetBottom() 
				&& player.previous->GetTop() >= tile_rect.GetBottom()) { //Player intersected from the bottom of the tile
					player.SetPosition(player.GetPosition().x, tile_rect.GetBottom());
					player.SetVelocity(player.GetVelocity().x,0.0f);
			}
			else if(player.current->GetRight() >= tile_rect.GetLeft()
				&& player.previous->GetRight() <= tile_rect.GetLeft()) { //Player intersected from the left
					player.SetPosition(tile_rect.GetLeft()-player.GetSize().i, player.GetPosition().y);
					player.move_right = false;
			}
			else if(player.current->GetLeft() <= tile_rect.GetRight()
				&& player.previous->GetLeft() >= tile_rect.GetRight()) { //Player intersected from the right
					player.SetPosition(tile_rect.GetRight(), player.GetPosition().y);
					player.move_left = false;		
			}
			
		}
		
	}
}
