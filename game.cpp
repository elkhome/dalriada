//#include <vector>
//#include <string>
#include <iostream>

#include "game.hpp"
#include "assets.hpp"
#include "world.cpp"

using namespace blit;
const int tilesize = 8;

const Rect player_front_stand = Rect(0,32,2,2);
const Rect player_front_walk = Rect(2,32,2,2);
const Rect player_side_stand = Rect(8,32,2,2);
const Rect player_side_walk = Rect(10,32,2,2);
const Rect player_back_stand = Rect(4,32,2,2);
const Rect player_back_walk = Rect(6,32,2,2);

//#define M_WIDTH 80
//#define M_HEIGHT 120

const int32_t map_width = M_WIDTH; //40;
const int32_t map_height = M_HEIGHT; //84;

enum Platform {
	BLIT,
	PICO,
};

Platform platform = BLIT;

const int scr_width = 20; //20 for 32blit, 15 for picosystem
const int scr_height = 15;
const int scr_area = (scr_width+1)*(scr_height+1);

Point tile_offset(18, 0);
Point camera_offset(0, 0);
Point player_offset(0,0);

int animationTimer = 0;
int currentFrame = 0;
int framecount = 1;

int momentum = 0;

enum Direction {
	LEFT	= 0b0001,
	RIGHT	= 0b0010,
	DOWN	= 0b0100,
	UP		= 0b1000,
	
};

enum State {
	WALKING,
	XMENU,
	DIALOG
};

State state = WALKING;

enum Camera {
	FREE,
	FIXED,
};

Camera camera = FREE;

const uint8_t collidemap[1024] = {
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,15,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,15,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,15,15,15,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,15,15,15,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,15,15,15,15,15,15,15,15,15,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,15,15,15,15,15,15,15,15,15,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,15,15,15,15,15,15,15,15,15,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,15,15,15,15,15,15,15,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,15,15,15,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,15,15,15,15,0,0,0,0,0,0,15,15,0,0,0,0,15,15,15,15,15,15,15,0,0,0,0,0,0,0,0,0,15,15,15,15,0,0,0,0,0,0,15,15,0,0,0,0,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,0,0,0,0,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,0,0,0,0,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,0,0,0,0,15,15,15,15,15,15,15,15,15,15,15,15,0,0,0,0,15,15,15,15,15,15,15,15,15,15,15,15,0,0,0,0,0,0,15,15,15,15,15,15,15,15,15,15,0,0,0,0,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,0,0,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,16,16,0,0,0,0,15,15,15,15,15,15,15,15,15,0,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,16,16,0,0,0,0,0,0,15,15,15,15,15,15,15,0,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,16,16,16,16,0,0,0,0,15,15,15,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,16,16,16,16,0,0,0,0,15,15,15,15,15,15,15,15,15,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,15,15,15,15,0,0,0,15,15,0,0,0,15,15,15,15,15,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,15,15,15,15,0,0,0,15,15,0,0,0,0,0,0,0,15,15,0,0,0,0,0,0,15,15,15,15,15,15,15,15,0,0,0,0,0,0,0,15,15,0,0,0,0,0,0,0,15,15,0,0,0,0,0,0,15,15,15,15,15,15,15,15,0,0,0,0,0,0,0,15,15,0,0,0,0,0,15,15,15,15,0,0,0,0,0,0,15,15,15,15,15,15,15,15,0,0,0,0,0,0,0,15,15,0,0,0,0,0,15,15,15,15,0,0,0,0,0,0,15,15,15,15,15,15,15,15,0,0,0,0,15,15,15,15,15,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,15,15,15,15,15,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,15,15,15,15,15,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,15,15,15,15,15,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

int trans(int x) { return ( x ) * tilesize; }
//int transy(int y) { return ( y ) * tilesize; }

int slidex(int j) { return ( j - tile_offset.x ) * tilesize + camera_offset.x; }
int slidey(int i) { return ( i - tile_offset.y ) * tilesize + camera_offset.y; }

struct Player {
	Point dir;
	Point pos;
	Direction facing;
	Direction last_facing;
	Rect sprite;
	Rect sprite_top;
	bool flip;
	bool alternate;
	bool alternate_flip;
	Rect animation[6];
	Rect animation_top[6];
	
	Player() {
		dir = Point(0, 1);
		pos = Point(6, 6);
		facing = DOWN;
		
		animation[0] = {player_front_stand};
		animation[1] = {player_front_walk};
		animation[2] = {player_back_stand};
		animation[3] = {player_back_walk};
		animation[4] = {player_side_stand};
		animation[5] = {player_side_walk};
		
		sprite = player_front_stand;
		sprite_top = Rect(2, 0, 2, 1);
		flip = 0;
		alternate = false;
	}

	void placeplayer() {
		switch(facing) {
		
		case LEFT:
			dir = Point(-1,0);
			if(momentum > 0) {
				flip = 1;
				//sprite = player_side_walk;
				sprite = (alternate) ? animation[5] : animation[4];
				if (facing != last_facing) sprite = animation[4]; //Override if this is the first time turning in this direction.
				sprite_top = (alternate) ? animation_top[5] : animation_top[4];
				if (facing != last_facing) sprite = animation_top[4]; //Override if this is the first time 
				//alternate = !alternate;
			} else {
				//alternate = 0;
				sprite = animation[4];
			}
			last_facing = facing;
			break;
		case RIGHT:
			dir = Point(1,0);
			if(momentum > 0) {	
				flip = 0;
				//sprite = player_side_walk;
				sprite = (alternate) ? animation[5] : animation[4];
				if (facing != last_facing) sprite = animation[4]; //Override if this is the first time turning in this direction.
				sprite_top = (alternate) ? animation_top[5] : animation_top[4];
				if (facing != last_facing) sprite = animation_top[4]; //Override if this is the first time 
				//alternate = !alternate;
			} else {
				//alternate = 0;
				sprite = animation[4];
			}
			last_facing = facing;
			break;
		case DOWN:
			dir = Point(0,1);
			//flip = 0;
			if(momentum > 0) {	
				flip = alternate;
				sprite = animation[1];
				sprite_top = animation_top[1];
			} else {
				//alternate = 0;
				sprite = animation[0];
			}
			last_facing = facing;
			break;
		case UP:
			dir = Point(0,-1);
			//flip = 0;
			if(momentum > 0) {	
				flip = alternate;
				sprite = animation[3];
				sprite_top = animation_top[3];
			} else {
				//alternate = 0;
				sprite = animation[2];
			}
			last_facing = facing;
			break;

	/*	case UP: //version of case from HeathenUK
			dir = Point(0,-1);
			flip = 0;
			//flip = alternate ? 0 : 1;
			alternate = !alternate;
			//sprite = player_back_stand;
			sprite = (facing == last_facing) ? animation[3] : animation[2];
			sprite_top = player_back_stand_top;
			last_facing = facing;
			break; */

		}
	}

	void drawplayer() {
		screen.sprite(sprite, Point(pos.x*8 + player_offset.x,pos.y*8 + player_offset.y), flip);
	}
	
	void drawplayer_top() {
		screen.sprite(sprite_top, Point(pos.x*8 + player_offset.x,pos.y*8 + player_offset.y), flip);
	}

} player;

struct Tilemap { //Class is a work in progress. Eventually, the map/collision/etc should be calculated here.
	Point pos;
	const int fringe = 2;
	
	Tilemap() {
		pos = Point(20, 26);
	}

	struct Tile {
		Point coords;
		int sprite;
		int layer;
	};
	
	void drawmap2() {
		for (int j = -1* fringe; j < scr_height + fringe; j++) {
			for (int i = -1* fringe; i < scr_width + fringe; i++) {
				//screen.sprite(loadedtiles2[j][i].sprite, Point(i*8+camera_offset.x, j*8+camera_offset.y));
				screen.sprite(world_data3[std::min(map_height, std::max((int32_t)0, j+pos.y))][std::min(map_width, std::max((int32_t)0, i+pos.x))], Point(i*8+camera_offset.x, j*8+camera_offset.y));
				//screen.sprite(world_data3[std::min((int32_t)M_HEIGHT, std::max((int32_t)0, j+pos.y))][std::min((int32_t)M_WIDTH, std::max((int32_t)0, i+pos.x))], Point(i*8+camera_offset.x, j*8+camera_offset.y));
			//	screen.sprite(world_data3[std::min((int32_t)M_HEIGHT, std::max((int32_t)0, j+pos.y))][std::min((int32_t)M_WIDTH, std::max((int32_t)0, i+pos.x))], Point(i*8+camera_offset.x, j*8+camera_offset.y));
				
			}
		}
	}
	
	void drawmap4() {
		int s = 0;
		for (int j = -1* fringe; j < scr_height + fringe; j++) {
			for (int i = -1* fringe; i < scr_width + fringe; i++) {
				s = world_data3[std::min(map_height, std::max((int32_t)0, j+pos.y))][std::min(map_width, std::max((int32_t)0, i+pos.x))];
				//screen.sprite(loadedtiles2[j][i].sprite, Point(i*8+camera_offset.x, j*8+camera_offset.y));
				if (s == 640)
					{screen.sprite(1036, Point(i*8+camera_offset.x, j*8+camera_offset.y));}
				if (s == 641)
					{screen.sprite(1037, Point(i*8+camera_offset.x, j*8+camera_offset.y));}
				if (s == 642)
					{screen.sprite(1038, Point(i*8+camera_offset.x, j*8+camera_offset.y));}
				if (s == 643)
					{screen.sprite(1039, Point(i*8+camera_offset.x, j*8+camera_offset.y));}
			//	screen.sprite(world_data3[std::min((int32_t)M_HEIGHT, std::max((int32_t)0, j+pos.y))][std::min((int32_t)M_WIDTH, std::max((int32_t)0, i+pos.x))], Point(i*8+camera_offset.x, j*8+camera_offset.y));
				
			}
		}
	}
	
	int collidetiles(int n = 0) {
		
		int cx = 0;
		int cy = 0;
		
		if (n == 1) {
			switch(player.facing) { //one ahead, left foot
				case LEFT:
					cx = player.pos.x - 1;
					cy = player.pos.y + 1;
					break;
				case RIGHT:
					cx = player.pos.x + 2;
					cy = player.pos.y + 0;
					break;
				case DOWN:
					cx = player.pos.x + 1;
					cy = player.pos.y + 2;
					break;
				case UP:
					cx = player.pos.x + 0;
					cy = player.pos.y - 1;
					break;
			}
		} else if (n == 2) {
			switch(player.facing) { //one ahead, right foot
				case LEFT:
					cx = player.pos.x - 1;
					cy = player.pos.y + 0;
					break;
				case RIGHT:
					cx = player.pos.x + 2;
					cy = player.pos.y + 1;
					break;
				case DOWN:
					cx = player.pos.x + 0;
					cy = player.pos.y + 2;
					break;
				case UP:
					cx = player.pos.x + 1;
					cy = player.pos.y - 1;
					break;
			}
		} else if (n == 3) { //under left foot
			switch(player.facing) {
				case LEFT:
					cx = player.pos.x + 0;
					cy = player.pos.y + 1;
					break;
				case RIGHT:
					cx = player.pos.x + 1;
					cy = player.pos.y + 0;
					break;
				case DOWN:
					cx = player.pos.x + 1;
					cy = player.pos.y + 1;
					break;
				case UP:
					cx = player.pos.x + 0;
					cy = player.pos.y + 0;
					break;
			}
		} else if (n == 4) { //under right foot
			switch(player.facing) {
				case LEFT:
					cx = player.pos.x + 0;
					cy = player.pos.y + 0;
					break;
				case RIGHT:
					cx = player.pos.x + 1;
					cy = player.pos.y + 1;
					break;
				case DOWN:
					cx = player.pos.x + 0;
					cy = player.pos.y + 1;
					break;
				case UP:
					cx = player.pos.x + 1;
					cy = player.pos.y + 0;
					break;
			}
		} else { //two ahead, left foot
			switch(player.facing) {
				case LEFT:
					cx = player.pos.x - 2;
					cy = player.pos.y + 1;
					break;
				case RIGHT:
					cx = player.pos.x + 3;
					cy = player.pos.y + 0;
					break;
				case DOWN:
					cx = player.pos.x + 1;
					cy = player.pos.y + 3;
					break;
				case UP:
					cx = player.pos.x ;
					cy = player.pos.y - 2;
					break;
			}
		}
		return world_data3[pos.y+cy][pos.x+cx];
	};
	
	void teleport() {
		int loc_x = pos.x + player.pos.x;
		int loc_y = pos.y + player.pos.y;
		
		if (loc_x == 16 && loc_y == 28) {			//to crate room
			camera = FIXED;
			pos.x = 141 - 9;
			pos.y = 19 - 13;
			player.pos.x = 9;
			player.pos.y = 13;
			player.facing = UP;
		} else if (loc_x == 26 && loc_y == 32) {	//to rail room
			camera = FIXED;
			pos.x = 140 - player.pos.x;
			pos.y = 47 - 13;
			player.pos.y = 13;
			player.facing = UP;
		} else if (loc_x == 141 && loc_y == 19) {	//from crate room
			camera = FREE;
			pos.x = 16 - 6;
			pos.y = 28 - 6;
			player.facing = DOWN;
		} else if (loc_x == 140 && loc_y == 47) {	//from rail room
			camera = FREE;
			pos.x = 26 - 6;
			pos.y = 32 - 6;
			player.facing = DOWN;
		} else {
			
			pos.x = 14;
			pos.y = 18;
			player.facing = DOWN;
		}
	};

	bool collide() {
		
		int i = collidetiles(0);
		int j = collidetiles(1);
		int k = collidetiles(2);
		int l = collidetiles(3);
		int m = collidetiles(4);
		
		if (l == 0 || l == 460 || l == 480) {
			teleport();
		} else if (collidemap[i] == 15 || collidemap[k] == 15) {
			return true;
		} else if (collidemap[j] == 16 && collidemap[k] == 16) {
			return true;
		} else if (collidemap[l] == 16 && collidemap[m] == 16) {
			return true;
		} else {return false;}
		
		return false;
	}


} tilemap;

void actions(int n) {
	switch(n) {
		case 736: //noticeboard
			state = DIALOG;
			break;
		case 738: //signpost
			state = DIALOG;
			break;
		case 839: //chest
			state = DIALOG;
			break;
		case 212: //left man
			state = DIALOG;
			break;
		case 184: //small man
			state = DIALOG;
			break;
		case 214: //right man
			state = DIALOG;
			break;
		case 460: //right man
			state = DIALOG;
			break;
		case 480: //right man
			state = DIALOG;
			break;
	}
};

std::string actionlookup(Point n) {
	//Input tilemap coordinates of "actioned" object.
	
	//Search for dialog matching the designated coordinates.
	switch(n.x) {
		case 10:
			switch(n.y) {
				case 20:
					return "Headless Guy.";
					break;
			}
			break;
		case 12:
			switch(n.y) {
				case 12:
					return "Left Guy.";
					break;
			}
			break;
		case 14:
			switch(n.y) {
				case 12:
					return "Small Guy.";
					break;
				case 40:
					return "<- Left Right ->.";
					break;
			}
			break;
		case 16:
			switch(n.y) {
				case 12:
					return "Right Guy.";
					break;
			}
			break;
		case 24:
			switch(n.y) {
				case 22:
					return "It's a chest!";
					break;
			}
			break;
		case 28:
			switch(n.y) {
				case 20:
					return "This one has a tongue...";
					break;
			}
			break;
		case 30:
			switch(n.y) {
				case 20:
					return "It's painted rock?";
					break;
			}
			break;
		case 34:
			switch(n.y) {
				case 30:
					return "Post No Bills.";
					break;
			}
			break;
		case 133:
			switch(n.y) {
				case 9:
					return "Left Chest.";
					break;
			}
			break;
		case 135:
			switch(n.y) {
				case 9:
					return "Middle Chest.";
					break;
			}
			break;
		case 137:
			switch(n.y) {
				case 9:
					return "Right Chest.";
					break;
			}
			break;
		case 138:
			switch(n.y) {
				case 41:
					return "Left Chest.";
					break;
			}
			break;
		case 142:
			switch(n.y) {
				case 41:
					return "Middle Chest.";
					break;
			}
			break;
		case 146:
			switch(n.y) {
				case 41:
					return "Right Chest.";
					break;
			}
			break;
	}
	return "There is no dialog for this coordinate.";
}


///////////////////////////////////////////////////////////////////////////
// init()		// setup your game here

void init() {
    set_screen_mode(ScreenMode::lores);
    screen.sprites = SpriteSheet::load(asset_route01);
	
	momentum = tilesize * 2;
}


///////////////////////////////////////////////////////////////////////////
// render(time)		This function is called to perform rendering of the game. time is the amount if milliseconds elapsed since the start of your game

void render(uint32_t time) {

    //Set screen to default color.
	screen.pen = Pen(248, 248, 248);
    screen.clear();

	//Draw layer one of the map.
	tilemap.drawmap2();

	//Place playe
	player.drawplayer();
	
	tilemap.drawmap4();
	
	if(state == DIALOG) {
		screen.rectangle(Rect(8,16*4+8,scr_width*8-16,16*3-8));
		screen.pen = Pen(0, 0, 0);
		screen.text(actionlookup(Point(tilemap.pos.x + player.pos.x,tilemap.pos.y + player.pos.y - 2)), minimal_font, Rect(8+1,16*4+8,scr_width*8-16,16*3-8), true, center_center);
	}
	
	if(state == XMENU) {
		screen.rectangle(Rect(8,8,scr_width*8-16,scr_height*8-16));
		screen.pen = Pen(0, 0, 0);
		screen.text("Dictionary", minimal_font, Point(16,16), true, center_left);
		screen.text("Peppers", minimal_font, Point(16,16+8*1), true, center_left);
		screen.text("Save", minimal_font, Point(16,16+8*2), true, center_left);
	}
	
	if(platform == PICO){screen.rectangle(Rect(8*15,0,8*5,8*15));}
}


///////////////////////////////////////////////////////////////////////////
// update(time)		// This is called to update your game state. time is the amount if milliseconds elapsed since the start of your game

void update(uint32_t time) {
	
	if(state == WALKING) {
			
		if(momentum == 0) {
			
			if(camera_offset.x != 0) {
				camera_offset.x = 0;
				tilemap.pos.x += player.dir.x * 2;
			}

			if(camera_offset.y != 0) {
				camera_offset.y = 0;
				tilemap.pos.y += player.dir.y * 2;
			}
		
			if(buttons.state & DPAD_LEFT) {
				player.facing = LEFT;
				player.placeplayer();
				if(tilemap.collide() == false) {momentum = tilesize * 2;
					player.alternate = !player.alternate;
				}
			} else
			if(buttons.state & DPAD_RIGHT) {
				player.facing = RIGHT;
				player.placeplayer();
				if(tilemap.collide() == false) {momentum = tilesize * 2;
					player.alternate = !player.alternate;
				}
			} else
			if(buttons.state & DPAD_DOWN) {
				player.facing = DOWN;
				player.placeplayer();
				if(tilemap.collide() == false) {momentum = tilesize * 2;
					player.alternate = !player.alternate;
				}
			} else
			if(buttons.state & DPAD_UP) {
				player.facing = UP;
				player.placeplayer();
				if(tilemap.collide() == false) {momentum = tilesize * 2;
					player.alternate = !player.alternate;
				}
				
			} 
			if(buttons.pressed & A) {
				std::cout << "(";
				std::cout << tilemap.pos.x + player.pos.x;
				std::cout << ", ";
				std::cout << tilemap.pos.y + player.pos.y - 2;
				std::cout << ") ";
				actions(tilemap.collidetiles());
			}
			if(buttons.pressed & B) {
				currentFrame++;
				currentFrame %= framecount;
				player.placeplayer();
			}
			if(buttons.pressed & X) {
				state = XMENU;
			}
			if(buttons.pressed & Y) {
				switch(camera) {
					case FREE:
						camera = FIXED;
						break;
					case FIXED:
						camera = FREE;
						break;
				}
			}
			
		}
		
		if(momentum > 0) {
			if (camera == FREE) {
				player.pos = Point(6, 6);
				camera_offset.x -= player.dir.x;
				camera_offset.y -= player.dir.y;
				momentum--;
				player.placeplayer();
				if(abs(camera_offset.x) == 16) {
					camera_offset.x = 0;
					tilemap.pos.x += player.dir.x * 2;
				}
				if(abs(camera_offset.y) == 16) {
					camera_offset.y = 0;
					tilemap.pos.y += player.dir.y * 2;
				}
			} else {
				switch(platform) {
					case BLIT:
						player_offset.x += player.dir.x;
						player_offset.y += player.dir.y;
						momentum--;
						player.placeplayer();
						if(abs(player_offset.x) == 16) {
							player_offset.x = 0;
							player.pos.x += player.dir.x * 2;
						}
						if(abs(player_offset.y) == 16) {
							player_offset.y = 0;
							player.pos.y += player.dir.y * 2;
						}
						break;
					case PICO:
						if((player.pos.x <= 2 && player.facing == LEFT)|| (player.pos.x >= 12 && player.facing == RIGHT)) {
							camera_offset.x -= player.dir.x;
							} else {
								player_offset.x += player.dir.x;}
						player_offset.y += player.dir.y;
						momentum--;
						player.placeplayer();
						if(abs(camera_offset.x) == 16) {
							camera_offset.x = 0;
							tilemap.pos.x += player.dir.x * 2;
						}
						if(abs(player_offset.x) == 16) {
							player_offset.x = 0;
							player.pos.x += player.dir.x * 2;
						}
						if(abs(player_offset.y) == 16) {
							player_offset.y = 0;
							player.pos.y += player.dir.y * 2;
						}
						break;
				}
				
			}
		}
	} else if(state == DIALOG) {
		if(buttons.pressed & A) {
				state = WALKING;
			}
	} else if(state == XMENU) {
		if(buttons.pressed & X) {
				state = WALKING;
		}
	}
	
}
