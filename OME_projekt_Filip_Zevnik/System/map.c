/*
 * map.cpp
 *
 *  Created on: 16 Apr 2023
 *      Author: zevni
 */
#include "map.h"
#include "podatki.h"
#include "pomozne_funkcije.h"
#include "stdint.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

void generateMap(){
	uint8_t walkx = 12;
	uint8_t walky = 12;
	const uint8_t part_size = 24;
//	uint8_t part[24][24] = {1};
	uint8_t part[part_size][part_size];
	for(int y=0;y<24;y++){
	        for(int x=0;x<24;x++){
	            part[y][x] = 1;
	        }
		}
	memset(map, 1, sizeof(map));
	const int max_i = 250;
	int dir;
	int dirx, diry;
	for(int i=0;i<max_i;i++){
		dir = rand()%4;

		switch(dir){
		case 0:
			dirx = -1;
			diry = 0;
			break;
		case 1:
			dirx = 1;
			diry = 0;
			break;
		case 2:
			diry = 1;
			dirx = 0;
			break;

		case 3:
			diry = -1;
			dirx = 0;
			break;

		default:
			return;

		}
		if(walkx+dirx < 1 || walky+diry < 1 || walkx+dirx >= part_size || walky+diry >= part_size) i--;
		else{
			walkx += dirx;
			walky += diry;
			part[walky][walkx] = 0;
		}



	}

	//prekopira level v mapo
	for(int y=0;y<24;y++){
		for(int x=0;x<24;x++){
			map[y][25+x] = part[y][x];
		}
	}
	//genenrira zacetni prostor
	for(int y=1;y<26;y++){
		for(int x=1;x<26;x++){
			if(x > 17 && x < 25 && y > 6 && y < 18)
			map[y][x] = 0;
		}
	}
	//generira pot do levela
	int pathx = 25, pathy = 12;
	for(int i=0;i<13;i++ ){
		map[pathy][pathx] = 0;
		pathx++;
	}
}

void drawMiniMap(){
	int dpx = ((int)px/Blocksize);
	int dpy = ((int)py/Blocksize);
	for(int y=0;y<map_height*2;y+=2){
		for(int x=0;x<map_width*2;x+=2){
			uint16_t color;
			if(y == 0 || x == 0 || y == map_height*2-2 || x == map_width*2-2){
				color = create_rgb(255, 71, 71);
			}else{
				if(map[y/2][x/2] == 0) color = create_rgb(255, 255, 255);
				else color = create_rgb(71, 71, 71);
				if(x/2 == dpx && y/2 == dpy) color = create_rgb(0, 0, 0);
				for(int i=0;i<num_of_enemys;i++){
					if(sp[i].life < 1) continue;
					int spx = sp[i].x / Blocksize;
					int spy = sp[i].y / Blocksize;
					if(x/2== spx && y/2 == spy) color = create_rgb(255, 0, 0);
				}
			}
			//UG_FillFrame(x*3, y*3, x*3+3, y*3+3, color);
			pixels[y*240 + x + 70] = color;
			pixels[y*240 + x+1 + 70] = color;
			pixels[(y+1)*240 + x + 70] = color;
			pixels[(y+1)*240 + x+1 + 70] = color;
		}
	}
	/*int dpx = ((int)px/Blocksize) * 2;
	int dpy = ((int)py/Blocksize) * 2;
	pixels[dpy*map_width + dpx] = create_rgb(0, 0, 0);
	pixels[dpy*map_width + dpx+1] = create_rgb(0, 0, 0);
	pixels[(dpy+1)*map_width*2 + dpx] = create_rgb(0, 0, 0);
	pixels[(dpy+1)*map_width*2 + dpx+1] = create_rgb(0, 0, 0);*/
}

