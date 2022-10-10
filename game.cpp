//#include <vector>
//#include <string>
#include <iostream>

#include "game.hpp"
#include "assets.hpp"
#include "world.cpp"

using namespace blit;
const int tilesize = 8;

const Rect player_front_stand = Rect(28,8,2,2);
const Rect player_front_walk = Rect(28,8,2,2);
const Rect player_side_stand = Rect(28,8,2,2);
const Rect player_side_walk = Rect(28,8,2,2);
const Rect player_back_stand = Rect(28,8,2,2);
const Rect player_back_walk = Rect(28,8,2,2);

const Rect player_front_stand_top = Rect(2,0,2,1);
const Rect player_front_walk_top = Rect(0,0,2,1);
const Rect player_side_stand_top = Rect(8,0,2,1);
const Rect player_side_walk_top = Rect(10,0,2,1);
const Rect player_back_stand_top = Rect(6,0,2,1);
const Rect player_back_walk_top = Rect(4,0,2,1);

#define M_WIDTH 80
#define M_HEIGHT 120

int map_width = M_WIDTH; //40;
int map_height = M_HEIGHT; //84;

const int scr_width = 20; //20 for 32blit, 15 for picosystem
const int scr_height = 15;
const int scr_area = (scr_width+1)*(scr_height+1);

Point tile_offset(18, 0);
Point camera_offset(0, 0);

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

const uint8_t collidemap[1008] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,15,8,8,8,8,15,16,16,16,16,16,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,2,16,0,0,0,0,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,2,16,0,0,0,0,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,2,16,0,0,0,0,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,2,16,0,0,0,0,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,15,4,4,4,4,15,16,16,16,16,16,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,0,0,1,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,0,0,1,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,0,0,1,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,0,0,1,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,0,0,1,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,0,0,1,15,15,};

int trans(int x) { return ( x ) * tilesize; }
//int transy(int y) { return ( y ) * tilesize; }

int slidex(int j) { return ( j - tile_offset.x ) * tilesize + camera_offset.x; }
int slidey(int i) { return ( i - tile_offset.y ) * tilesize + camera_offset.y; }

void animate() {
	animationTimer += 1;
	int FRAME_LENGTH = 2;

	if (animationTimer >= FRAME_LENGTH) {
		animationTimer -= FRAME_LENGTH;
		currentFrame++;
		currentFrame %= framecount;
	}
}

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
		
		animation_top[0] = {player_front_stand_top};
		animation_top[1] = {player_front_walk_top};
		animation_top[2] = {player_back_stand_top};
		animation_top[3] = {player_back_walk_top};
		animation_top[4] = {player_side_stand_top};
		animation_top[5] = {player_side_walk_top};
		
		sprite = player_front_stand;
		sprite_top = Rect(2, 0, 2, 1);
		flip = 0;
		alternate = false;
	}

	void placeplayer() {
		switch(facing) {
		
		case LEFT:
			dir = Point(-1,0);
			flip = 0;
			//sprite = player_side_walk;
			sprite = (alternate) ? animation[5] : animation[4];
			if (facing != last_facing) sprite = animation[4]; //Override if this is the first time turning in this direction.
			sprite_top = (alternate) ? animation_top[5] : animation_top[4];
			if (facing != last_facing) sprite = animation_top[4]; //Override if this is the first time 
			//alternate = !alternate;
			last_facing = facing;
			break;
		case RIGHT:
			dir = Point(1,0);
			flip = 1;
			//sprite = player_side_walk;
			sprite = (alternate) ? animation[5] : animation[4];
			if (facing != last_facing) sprite = animation[4]; //Override if this is the first time turning in this direction.
			sprite_top = (alternate) ? animation_top[5] : animation_top[4];
			if (facing != last_facing) sprite = animation_top[4]; //Override if this is the first time 
			//alternate = !alternate;
			last_facing = facing;
			break;
		case DOWN:
			dir = Point(0,1);
			//flip = 0;
			flip = alternate_flip;
			sprite = (alternate) ? animation[1] : animation[0];
			sprite_top = (alternate) ? animation_top[1] : animation_top[0];
			if (facing != last_facing) {
				sprite = animation[0]; //Override if this is the first time turning in this direction.
				sprite = animation_top[0];
				//alternate_flip = 0;
			}//Override if this is the first time 
			//if (facing != last_facing) 
			if (alternate) {alternate_flip = !alternate_flip;}
			//alternate = !alternate;
			
			last_facing = facing;
			
			break;
		case UP:
			dir = Point(0,-1);
			//flip = 0;
			flip = alternate_flip;
			sprite = (alternate) ? animation[3] : animation[2];
			sprite_top = (alternate) ? animation_top[3] : animation_top[2];
			if (facing != last_facing) {
				sprite = animation[2]; //Override if this is the first time turning in this direction.
				sprite = animation_top[2];
				//alternate_flip = 0;
			}//Override if this is the first time 
			//if (facing != last_facing) 
			if (alternate) {alternate_flip = !alternate_flip;}
			//alternate = !alternate;
			
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
		screen.sprite(sprite, Point(pos.x*8,pos.y*8), flip);
	}
	
	void drawplayer_top() {
		screen.sprite(sprite_top, Point(pos.x*8,pos.y*8), flip);
	}

} player;

struct Tilemap { //Class is a work in progress. Eventually, the map/collision/etc should be calculated here.
	Point pos;
	const int fringe = 2;
	
	Tilemap() {
		pos = Point(6, 88);
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
				screen.sprite(world_data3[std::min((int32_t)M_HEIGHT, std::max((int32_t)0, j+pos.y))][std::min((int32_t)M_WIDTH, std::max((int32_t)0, i+pos.x))], Point(i*8+camera_offset.x, j*8+camera_offset.y));
				
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

	bool collide() {
		
		int i = collidetiles(0);
		int j = collidetiles(1);
		int k = collidetiles(2);
		int l = collidetiles(3);
		int m = collidetiles(4);
		
		if (collidemap[i] == 15 || collidemap[k] == 15) {
			return true;
		} else if (collidemap[j] == 16 && collidemap[k] == 16) {
			return true;
		} else if (collidemap[l] == 16 && collidemap[m] == 16) {
			return true;
		} else {return false;}
		
		//return false;
	}


} tilemap;

void actions(int n) {
	switch(n) {
		case 854: //grave
			state = DIALOG;
			break;
		case 758: //door
			state = DIALOG;
			break;
		case 856: //chest
			state = DIALOG;
			break;
	}
};

std::string actionlookup(Point n) {
	//Input tilemap coordinates of "actioned" object.
	
	//Search for dialog matching the designated coordinates.
	switch(n.x) {
		case 4:
			switch(n.y) {
				case 6:
					return "Corpse #3.";
					break;
			}
		case 6:
			switch(n.y) {
				case 2:
					return "Knock knock!";
					break;
			}
		case 10:
			switch(n.y) {
				case 2:
					return "Fake Plastic Corpse.";
					break;
			}
		case 14:
			switch(n.y) {
				case 2:
					return "This one's just a neat rock.";
					break;
			}
		case 16:
			switch(n.y) {
				case 2:
					return "RIP 196X - 199X.";
					break;
			}
		case 12:
			switch(n.y) {
				case 92:
					return "Knock knock!";
					break;
			}
		case 60:
			switch(n.y) {
				case 90:
					return "Corpse #1.";
					break;
			}
		case 62:
			switch(n.y) {
				case 86:
					return "Corpse #2.";
					break;
			}
		case 66:
			switch(n.y) {
				case 84:
					return "Help! I'm stuck!.";
					break;
			}
	}
	return "There is no dialog for this coordinate.";
}


///////////////////////////////////////////////////////////////////////////
// init()		// setup your game here

void init() {
    set_screen_mode(ScreenMode::lores);
    screen.sprites = SpriteSheet::load(asset_route01);
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
			
			//Collision triggers here doesn't do anything.
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
				if(tilemap.collide() == false) {momentum = tilesize * 2;}
					else if(tilemap.collidetiles() >= 58 && tilemap.collidetiles() <= 60) {
						momentum = tilesize * 4;
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
				player.sprite = player.animation[0];
			}
			
		}
		
		if(momentum > 0) {
			camera_offset.x -= player.dir.x;
			camera_offset.y -= player.dir.y;
			momentum--;
			animate();
			player.placeplayer();
			if(abs(camera_offset.x) == 16) {
				camera_offset.x = 0;
				tilemap.pos.x += player.dir.x * 2;
			}
			if(abs(camera_offset.y) == 16) {
				camera_offset.y = 0;
				tilemap.pos.y += player.dir.y * 2;
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
