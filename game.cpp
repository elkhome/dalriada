#include <vector>
#include <string>
#include <iostream>

#include "game.hpp"
#include "assets.hpp"

using namespace blit;
const int tilesize = 8;

const Rect player_front_stand = Rect(2,0,2,2);
const Rect player_front_walk = Rect(0,0,2,2);
const Rect player_side_stand = Rect(8,0,2,2);
const Rect player_side_walk = Rect(10,0,2,2);
const Rect player_back_stand = Rect(6,0,2,2);
const Rect player_back_walk = Rect(4,0,2,2);

const Rect player_front_stand_top = Rect(2,0,2,1);
const Rect player_front_walk_top = Rect(0,0,2,1);
const Rect player_side_stand_top = Rect(8,0,2,1);
const Rect player_side_walk_top = Rect(10,0,2,1);
const Rect player_back_stand_top = Rect(6,0,2,1);
const Rect player_back_walk_top = Rect(4,0,2,1);

int map_width = 40;
int map_height = 84;

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
	LEFT,
	RIGHT,
	DOWN,
	UP
};

enum State {
	WALKING,
	MENU,
	DIALOG
};

State state = WALKING;

const int world_data3[84][40] = {53,53,53,53,53,53,72,73,53,53,53,53,53,53,53,53,53,53,72,73,50,50,50,50,72,73,53,53,53,53,53,53,53,53,53,53,72,73,53,53,53,53,53,53,53,53,84,85,53,53,53,53,53,53,53,53,53,53,84,85,50,50,51,50,84,85,53,53,53,53,53,53,53,53,53,53,84,85,53,53,53,53,53,53,53,53,72,73,53,53,53,53,53,53,53,53,53,53,72,73,50,50,50,50,72,73,53,53,53,53,53,53,53,53,53,53,72,73,53,53,53,53,53,53,53,53,84,85,53,53,53,53,53,53,53,53,53,53,84,85,51,50,50,50,84,85,53,53,53,53,53,53,53,53,53,53,84,85,53,53,53,53,53,53,53,53,72,73,53,53,53,53,53,53,53,53,53,53,72,73,50,50,50,50,72,73,53,53,53,53,53,53,53,53,53,53,72,73,53,53,53,53,53,53,53,53,84,85,53,53,53,53,53,53,53,53,53,53,84,85,50,50,51,50,84,85,53,53,53,53,53,53,53,53,53,53,84,85,53,53,53,53,53,53,53,53,72,73,53,53,53,53,53,53,53,53,53,53,72,73,76,76,76,76,72,73,53,53,53,53,53,53,53,53,53,53,72,73,53,53,53,53,53,53,53,53,84,85,53,53,53,53,53,53,53,53,53,53,84,85,88,88,88,88,84,85,53,53,53,53,53,53,53,53,53,53,84,85,53,53,53,53,53,53,53,53,72,73,72,73,72,73,72,73,72,73,72,73,72,73,51,51,51,51,72,73,72,73,72,73,72,73,72,73,72,73,72,73,53,53,53,53,53,53,53,53,84,85,84,85,84,85,84,85,84,85,84,85,84,85,51,51,51,51,84,85,84,85,84,85,84,85,84,85,84,85,84,85,53,53,53,53,53,53,53,53,72,73,53,53,53,53,53,53,53,53,53,53,53,53,51,51,51,51,53,53,53,53,53,53,53,53,53,53,53,53,72,73,53,53,53,53,53,53,53,53,84,85,53,53,53,53,53,53,53,53,53,53,53,53,51,51,51,51,53,53,53,53,53,53,53,53,53,54,53,54,84,85,53,53,53,53,53,53,53,53,72,73,53,53,53,53,53,53,53,53,53,53,53,53,51,51,51,51,53,53,53,53,53,53,53,53,54,53,54,53,72,73,53,53,53,53,53,53,53,53,84,85,53,53,53,53,53,53,53,53,53,53,53,53,51,51,51,51,53,53,53,53,53,53,53,53,53,53,53,53,84,85,53,53,53,53,53,53,53,53,72,73,53,53,53,53,53,53,53,53,53,53,74,75,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,72,73,53,53,53,53,53,53,53,53,84,85,53,53,53,53,53,53,53,53,53,53,86,87,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,84,85,53,53,53,53,53,53,53,53,72,73,53,53,53,53,53,53,53,53,53,53,74,75,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,72,73,53,53,53,53,53,53,53,53,84,85,58,59,55,55,59,59,59,59,59,60,86,87,58,59,59,59,59,59,59,60,51,51,51,51,51,51,51,51,84,85,53,53,53,53,53,53,53,53,74,75,53,53,53,53,53,53,53,53,53,53,74,75,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,74,75,53,53,53,53,53,53,53,53,86,87,53,54,53,54,53,54,53,54,53,53,86,87,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,86,87,53,53,53,53,53,53,53,53,74,75,54,53,54,53,54,53,54,53,53,53,74,75,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,74,75,53,53,53,53,53,53,53,53,86,87,53,53,53,53,53,53,53,53,53,53,86,87,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,86,87,53,53,53,53,53,53,53,53,74,75,30,31,32,33,33,33,34,35,53,53,74,75,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,74,75,53,53,53,53,53,53,53,53,86,87,42,43,44,45,45,45,46,47,53,53,86,87,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,86,87,53,53,53,53,53,53,53,53,74,75,67,68,81,82,81,82,70,71,53,53,74,75,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,74,75,53,53,53,53,53,53,53,53,86,87,104,99,93,94,93,94,99,105,55,55,86,87,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,86,87,53,53,53,53,53,53,53,53,74,75,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,51,51,51,51,51,51,51,51,51,51,51,51,74,75,53,53,53,53,53,53,53,53,86,87,53,53,53,53,53,54,53,54,53,54,53,54,53,53,53,53,51,51,51,51,51,51,51,51,51,51,51,51,86,87,53,53,53,53,53,53,53,53,74,75,53,53,53,53,54,53,54,53,54,53,54,53,53,53,53,53,51,51,51,51,51,51,51,51,51,51,51,51,74,75,53,53,53,53,53,53,53,53,86,87,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,51,51,51,51,51,51,51,51,51,51,51,51,86,87,53,53,53,53,53,53,53,53,74,75,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,50,50,50,50,49,49,49,49,49,49,49,49,74,75,53,53,53,53,53,53,53,53,86,87,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,51,50,50,50,49,49,49,49,49,49,49,49,86,87,53,53,53,53,53,53,53,53,74,75,74,75,74,75,53,53,53,53,53,53,53,53,74,75,74,75,74,75,74,75,49,49,49,49,49,49,49,49,74,75,53,53,53,53,53,53,53,53,86,87,86,87,86,87,58,59,55,55,55,55,59,60,86,87,86,87,86,87,86,87,49,49,49,49,49,49,49,49,86,87,53,53,53,53,53,53,53,53,72,73,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,51,51,51,51,49,49,49,49,49,49,49,49,72,73,53,53,53,53,53,53,53,53,84,85,53,53,53,53,53,53,53,53,53,54,53,54,53,54,53,54,51,51,51,51,49,49,49,49,49,49,49,49,84,85,53,53,53,53,53,53,53,53,72,73,53,53,53,53,53,53,53,53,54,53,54,53,54,53,54,53,51,51,51,51,49,49,49,49,49,49,49,49,72,73,53,53,53,53,53,53,53,53,84,85,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,51,51,51,51,49,49,49,49,49,49,49,49,84,85,53,53,53,53,53,53,53,53,72,73,53,53,53,53,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,53,53,53,53,53,53,53,53,72,73,53,53,53,53,53,53,53,53,84,85,53,53,53,53,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,53,54,53,54,53,54,53,54,84,85,53,53,53,53,53,53,53,53,72,73,53,53,53,53,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,54,53,54,53,54,53,54,53,72,73,53,53,53,53,53,53,53,53,84,85,53,53,53,53,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,53,53,53,53,53,53,53,53,84,85,53,53,53,53,53,53,53,53,72,73,53,53,53,53,51,51,51,51,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,72,73,53,53,53,53,53,53,53,53,84,85,53,53,53,53,51,51,51,51,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,84,85,53,53,53,53,53,53,53,53,72,73,53,53,53,53,51,51,51,51,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,72,73,53,53,53,53,53,53,53,53,84,85,58,60,55,55,58,59,59,59,59,60,55,55,58,59,59,59,59,59,59,59,59,59,59,59,59,59,59,60,84,85,53,53,53,53,53,53,53,53,72,73,53,53,53,53,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,72,73,53,53,53,53,53,53,53,53,84,85,53,53,53,53,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,84,85,53,53,53,53,53,53,53,53,72,73,53,53,53,53,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,72,73,53,53,53,53,53,53,53,53,84,85,53,53,53,53,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,84,85,53,53,53,53,53,53,53,53,72,73,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,49,49,49,49,49,49,49,49,51,51,51,51,72,73,53,53,53,53,53,53,53,53,84,85,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,49,49,49,49,49,49,49,49,51,51,51,51,84,85,53,53,53,53,53,53,53,53,72,73,74,75,74,75,74,75,74,75,74,75,74,75,74,75,74,75,49,49,49,49,49,49,49,49,51,51,51,51,72,73,53,53,53,53,53,53,53,53,84,85,86,87,86,87,86,87,86,87,86,87,86,87,86,87,86,87,49,49,49,49,49,49,49,49,58,59,59,60,84,85,53,53,53,53,53,53,53,53,72,73,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,49,49,49,49,49,49,49,49,51,51,51,51,72,73,53,53,53,53,53,53,53,53,84,85,53,53,53,53,53,53,53,53,53,54,53,54,53,54,53,54,49,49,49,49,49,49,49,49,51,51,51,51,84,85,53,53,53,53,53,53,53,53,72,73,53,53,53,53,53,53,53,53,54,53,54,53,54,53,54,53,49,49,49,49,49,49,49,49,51,51,51,51,72,73,53,53,53,53,53,53,53,53,84,85,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,49,49,49,49,49,49,49,49,51,51,51,51,84,85,53,53,53,53,53,53,53,53,72,73,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,72,73,53,53,53,53,53,53,53,53,84,85,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,84,85,53,53,53,53,53,53,53,53,72,73,51,51,51,51,51,51,51,51,51,51,77,78,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,72,73,53,53,53,53,53,53,53,53,84,85,58,59,59,60,51,51,51,51,51,51,89,90,58,59,55,55,59,59,59,59,59,59,59,59,59,59,59,60,84,85,53,53,53,53,53,53,53,53,74,75,53,53,53,53,49,49,49,49,49,49,49,49,51,51,51,51,53,53,53,53,49,49,49,49,49,49,49,49,74,75,53,53,53,53,53,53,53,53,86,87,53,53,53,53,49,49,49,49,49,49,49,49,51,51,51,51,53,53,53,53,49,49,49,49,49,49,49,49,86,87,53,53,53,53,53,53,53,53,74,75,53,53,53,53,49,49,49,49,49,49,49,49,51,51,51,51,53,53,53,53,49,49,49,49,49,49,49,49,74,75,53,53,53,53,53,53,53,53,86,87,53,53,53,53,49,49,49,49,49,49,49,49,51,51,51,51,53,53,53,53,49,49,49,49,49,49,49,49,86,87,53,53,53,53,53,53,53,53,74,75,49,49,49,49,49,49,49,49,53,53,53,53,51,51,51,51,49,49,49,49,49,49,49,49,53,53,53,53,74,75,53,53,53,53,53,53,53,53,86,87,49,49,49,49,49,49,49,49,53,54,53,54,51,51,51,51,49,49,49,49,49,49,49,49,53,54,53,54,86,87,53,53,53,53,53,53,53,53,74,75,49,49,49,49,49,49,49,49,54,53,54,53,51,51,51,51,49,49,49,49,49,49,49,49,54,53,54,53,74,75,53,53,53,53,53,53,53,53,86,87,49,49,49,49,49,49,49,49,53,53,53,53,51,51,51,51,49,49,49,49,49,49,49,49,53,53,53,53,86,87,53,53,53,53,53,53,53,53,74,75,72,73,72,73,72,73,72,73,72,73,72,73,49,49,49,49,72,73,72,73,72,73,72,73,72,73,72,73,74,75,53,53,53,53,53,53,53,53,86,87,84,85,84,85,84,85,84,85,84,85,84,85,49,49,49,49,84,85,84,85,84,85,84,85,84,85,84,85,86,87,53,53,53,53,53,53,53,53,74,75,53,53,53,53,53,53,53,53,53,53,72,73,76,76,76,76,72,73,53,53,53,53,53,53,53,53,53,53,74,75,53,53,53,53,53,53,53,53,86,87,53,53,53,53,53,53,53,53,53,53,84,85,88,88,88,88,84,85,53,53,53,53,53,53,53,53,53,53,86,87,53,53,53,53,53,53,53,53,74,75,53,53,53,53,53,53,53,53,53,53,72,73,49,49,49,49,72,73,53,53,53,53,53,53,53,53,53,53,74,75,53,53,53,53,53,53,53,53,86,87,53,53,53,53,53,53,53,53,53,53,84,85,49,49,49,49,84,85,53,53,53,53,53,53,53,53,53,53,86,87,53,53,53,53,53,53,53,53,74,75,53,53,53,53,53,53,53,53,53,53,72,73,49,49,49,49,72,73,53,53,53,53,53,53,53,53,53,53,74,75,53,53,53,53,53,53,53,53,86,87,53,53,53,53,53,53,53,53,53,53,84,85,49,49,49,49,84,85,53,53,53,53,53,53,53,53,53,53,86,87,53,53,53,53,53,53,53,53,72,73,53,53,53,53,53,53,53,53,53,53,72,73,49,49,49,49,72,73,53,53,53,53,53,53,53,53,53,53,72,73,53,53,53,53,53,53,53,53,84,85,53,53,53,53,53,53,53,53,53,53,84,85,49,49,49,49,84,85,53,53,53,53,53,53,53,53,53,53,84,85,53,53,72,73,72,73,72,73,72,73,72,73,72,73,72,73,72,73,72,73,72,73,49,49,49,49,72,73,72,73,72,73,72,73,72,73,72,73,72,73,72,73,84,85,84,85,84,85,84,85,84,85,84,85,84,85,84,85,84,85,84,85,49,49,49,49,84,85,84,85,84,85,84,85,84,85,84,85,84,85,84,85,};

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
	Rect sprite;
	Rect sprite_top;
	bool flip;
	Rect animation[2];
	
	Player() {
		dir = Point(0, 1);
		pos = Point(6, 6);
		facing = DOWN;
		animation[0] = {player_front_stand};
		animation[1] = {player_front_walk};
		sprite = Rect(2, 0, 2, 2);
		sprite_top = Rect(2, 0, 2, 1);
		flip = 0;
	}

	void placeplayer() {
		switch(facing) {
		case LEFT:
			dir = Point(-1,0);
			flip = 0;
			sprite = player_side_walk;
			sprite_top = player_side_walk_top;
			break;
		case RIGHT:
			dir = Point(1,0);
			flip = 1;
			sprite = player_side_walk;
			sprite_top = player_side_walk_top;
			break;
		case DOWN:
			dir = Point(0,1);
			flip = 0;
			animation[0] = {player_front_stand};
			animation[1] = {player_front_walk};
			sprite = animation[0];
			sprite_top = player_front_stand_top;
			break;
		case UP:
			dir = Point(0,-1);
			flip = 0;
			sprite = player_back_stand;
			sprite_top = player_back_stand_top;
			break;
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
	
	Tilemap() {
		pos = Point(4, 20);
	}

	struct Tile {
		Point coords;
		int sprite;
		int layer;
	};
	
	//Tile loadedtiles[scr_area];
	//Tile loadedtiles2[scr_width+10][scr_height+10];


/*	void loadmap2() {
		for (int j = 0; j < scr_height; j++) {
			for (int i = 0; i < scr_width; i++) {
				loadedtiles2[j][i].sprite = world_data3[j][i];
			}
		}
	} */

/*	void drawmap() {
		int i = 0;
		while (i < scr_area) {
			screen.sprite(loadedtiles[i].sprite, Point(loadedtiles[i].coords.x, loadedtiles[i].coords.y));
			i++;
		}
	} */
	
	void drawmap2() {
		for (int j = 0; j < scr_height; j++) {
			for (int i = 0; i < scr_width; i++) {
				//screen.sprite(loadedtiles2[j][i].sprite, Point(i*8+camera_offset.x, j*8+camera_offset.y));
				screen.sprite(world_data3[j+pos.y][i+pos.x], Point(i*8+camera_offset.x, j*8+camera_offset.y));
			}
		}
	}
	
	void drawmap3() {
		for (int j = 0; j < scr_height; j++) {
			for (int i = 0; i < scr_width; i++) {
				//screen.sprite(loadedtiles2[j][i].sprite, Point(i*8+camera_offset.x, j*8+camera_offset.y));
				if(world_data3[j+pos.y][i+pos.x] == 49) {
					screen.sprite(world_data3[j+pos.y][i+pos.x], Point(i*8+camera_offset.x, j*8+camera_offset.y));
				}
			}
		}
	}
	
	void drawmap4() {
		for (int j = 0; j < scr_height; j++) {
			for (int i = 0; i < scr_width; i++) {
				//screen.sprite(loadedtiles2[j][i].sprite, Point(i*8+camera_offset.x, j*8+camera_offset.y));
				if(world_data3[j+pos.y][i+pos.x] >= 23 && world_data3[j+pos.y][i+pos.x] <= 48) {
					screen.sprite(world_data3[j+pos.y][i+pos.x], Point(i*8+camera_offset.x, j*8+camera_offset.y));
				}
			}
		}
	}
	
/*	void drawgrass() {
		int i = 0;
		while (i < scr_area) {
			if(loadedtiles[i].layer == 3) {
				screen.sprite(loadedtiles[i].sprite, Point(loadedtiles[i].coords.x, loadedtiles[i].coords.y));
			}
			i++;
		}
	} */

	int collidetiles(int n = 0) {
		
		int cx = 0;
		int cy = 0;
		
		if (n == 1) {
			switch(player.facing) {
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
		} else {
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

		if (collidetiles(0) >= 58 || collidetiles(1) >= 58) {
			return true;
		} else {return false;}
		
		//return false;
	}

} tilemap;

void actions(int n) {
	switch(n) {
		case 77: //sign
			state = DIALOG;
			break;
		case 81: //door
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
				case 24:
					return "This is the left door.";
					break;
			}
		case 12:
			switch(n.y) {
				case 24:
					return "This is the right door.";
					break;
			}
		case 18:
			switch(n.y) {
				case 60:
					return "This is a sign.";
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
	//tilemap.loadmap2();
}


///////////////////////////////////////////////////////////////////////////
// render(time)		This function is called to perform rendering of the game. time is the amount if milliseconds elapsed since the start of your game

void render(uint32_t time) {

    //Set screen to default color.
	screen.pen = Pen(248, 248, 248);
    screen.clear();

	//Draw layer one of the map.
	tilemap.drawmap2();
	//tilemap.drawmap();

	//Place Flowers #TODO FIX ANIMATION

	//Place player's bottom half.
	player.drawplayer();
	
	//Place long grass.
	//tilemap.drawgrass();
	tilemap.drawmap3();

	//Place player's top half.
	player.drawplayer_top();
	
	//Place building tops
	tilemap.drawmap4();
	
	
	if(state == DIALOG) {
		screen.rectangle(Rect(8,16*4+8,scr_width*8-16,16*3-8));
		screen.pen = Pen(0, 0, 0);
		screen.text(actionlookup(Point(tilemap.pos.x + player.pos.x,tilemap.pos.y + player.pos.y - 2)), minimal_font, Rect(8+1,16*4+8,scr_width*8-16,16*3-8), true, center_center);
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
				if(tilemap.collide() == false) {momentum = tilesize * 2;}
			}
			if(buttons.state & DPAD_RIGHT) {
				player.facing = RIGHT;
				player.placeplayer();
				if(tilemap.collide() == false) {momentum = tilesize * 2;}
			}
			if(buttons.state & DPAD_DOWN) {
				player.facing = DOWN;
				player.placeplayer();
				if(tilemap.collide() == false) {momentum = tilesize * 2;}
					else if(tilemap.collidetiles() >= 58 && tilemap.collidetiles() <= 60) {
						momentum = tilesize * 4;
					}
			}
			if(buttons.state & DPAD_UP) {
				player.facing = UP;
				player.placeplayer();
				if(tilemap.collide() == false) {momentum = tilesize * 2;}
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
				player.sprite = player.animation[1];
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
	}
	//tilemap.loadmap();
	//tilemap.loadmap2();
	
}
