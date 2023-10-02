/*
 * podatki.c
 *
 *  Created on: May 16, 2022
 *      Author: zevni
 */

#include "podatki.h"

#include "stdint.h"


/*int PI = 3.14159;
int DEGREE = 0.01745329;*/

//DIMENZIJE IGRALNEGA POLJA
/*int width = 100;
int height = 100;

int s_width = 320;
int s_height = 240;
int s_scale = 2.4;
int s_offset = 0;*/


//DEFINICIJE ZA IGRALCA
float px = width/2;
float py = height/2;
float angle = PI;
int r = 0;
int f = 0;
int interact = 0;

//TRENUTNE BARVE

int rd = 0, g = 0, b = 0;
//DIMENZIJE ENE CELICE
int Blocksize = 10;

//SEZNAM SLIKIC
sprite sp[2];

//DEFINICIJA MAPE
uint8_t map[10][10] = {
		{1,1,1,1,1,1,1,1,1,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,2,0,0,0,0,1},
		{1,0,0,0,0,0,0,2,0,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,2,3,2,2,2,2,0,0,1},
		{1,0,0,0,0,0,2,0,0,1},
		{1,0,0,0,0,0,2,0,0,1},
		{1,1,1,1,1,1,1,1,1,1}
};

uint16_t pixels[57600];
