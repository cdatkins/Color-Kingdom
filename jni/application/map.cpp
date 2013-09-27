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
				new_tile->id = Zeni::Point2f(i,j);
				new_tile->tile_type = tile_type;
				new_tile->contain_entity = false;
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
	
	//player.activate_gravity = true;
	for(int i = 0; i < tiles.size(); i++) {
		
		if(player.current->Intersects(*tiles[i]->rect)) {
			Rect tile_rect = *tiles[i]->rect;
			player.tile_id = tiles[i]->id;

			if(player.current->GetBottom() >= tile_rect.GetTop() 
				&& player.previous->GetBottom() <= tile_rect.GetTop()) { //Player intersected from the top of the tile
					player.SetPosition(player.GetPosition().x,tile_rect.GetTop() - player.GetSize().j);
					player.activate_gravity = false;
					tiles[i]->contain_entity = true;
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

		if(tiles[i]->contain_entity && (tiles[i]->id.x == player.tile_id.x) && (tiles[i]->id.y == player.tile_id.y)) {
			
			if(player.current->GetRight() < tiles[i]->rect->GetLeft() 
				|| player.current->GetLeft() > tiles[i]->rect->GetRight()) {
					tiles[i]->contain_entity = false;
					player.activate_gravity = true;
			}
		}
		
	}

	
}

void Map::Collision(Orb &orb) {

	for(int i = 0; i < tiles.size(); i++) {
		
		if(orb.current->Intersects(*tiles[i]->rect)) {
			
			Rect tile_rect = *tiles[i]->rect;
			
			if(orb.current->GetBottom() >= tile_rect.GetTop() 
				&& orb.previous->GetBottom() <= tile_rect.GetTop()) { //Orb intersected from the top of the tile
					orb.OnCollision(Rect::TOP);
			}
			else if(orb.current->GetTop() <= tile_rect.GetBottom() 
				&& orb.previous->GetTop() >= tile_rect.GetBottom()) { //Orb intersected from the bottom of the tile
					orb.OnCollision(Rect::BOTTOM);
			}
			else if(orb.current->GetRight() >= tile_rect.GetLeft()
				&& orb.previous->GetRight() <= tile_rect.GetLeft()) { //Orb intersected from the left
					orb.OnCollision(Rect::LEFT);
			}
			else if(orb.current->GetLeft() <= tile_rect.GetRight()
				&& orb.previous->GetLeft() >= tile_rect.GetRight()) { //Orb intersected from the right
					orb.OnCollision(Rect::RIGHT);	
			}
			
		}
	}
}