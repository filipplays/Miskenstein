/*
 * podatki.h
 *
 *  Created on: May 16, 2022
 *      Author: zevni
 */

#ifndef INCLUDE_PODATKI_H_
#define INCLUDE_PODATKI_H_
#include "stdint.h"


typedef struct
{
	int tip;
	int map; //katera tekstura
	float x, y, z; //pozicija
	float sizeX, sizeY;
	int life;
	int damadgedTime;
	int damadgedTimeMax;
	float dist;
} sprite;


//DEFINICIJE ZA IGRALCA
extern float px;
extern float py;
extern float angle;
extern int r;
extern int f;
extern int interact;

//TRENUTNE BARVE

extern int rd, g, b;
//DIMENZIJE ENE CELICE
extern int Blocksize;

//SEZNAM SLIKIC
extern sprite sp[2];

//DEFINICIJA MAPE
extern uint8_t map[10][10];

extern uint16_t pixels[57600];


extern int PI;
extern int DEGREE;

//DIMENZIJE IGRALNEGA POLJA
#define width 100
#define height 100

#define s_width 320
#define s_height 240
#define s_scale 2.4
#define s_offset 0

//DEFINIRANE KONSTANTE
#define PI 3.14159
#define DEGREE 0.01745329


#endif /* INCLUDE_PODATKI_H_ */
