/*
 * podatki.h
 *
 *  Created on: May 16, 2022
 *      Author: zevni
 */

#ifndef INCLUDE_PODATKI_H_
#define INCLUDE_PODATKI_H_
#include "stdint.h"
#include "timing_utils.h"
#include "periodic_services.h"
#define num_of_blood_particles 20
#define gravity 0.5
#define maxspeed 5
#define friction 0.95

typedef struct
{
    float posx;
    float posy;
    float velx;
    float vely;
    uint8_t active;
} Blood;

typedef struct
{
	int tip;
	int map; //katera tekstura
	float x, y, z; //pozicija
	uint16_t sizeX, sizeY;
	uint8_t life;
	int damadgedTime;
	int damadgedTimeMax;
	float dist;
	Blood blood[num_of_blood_particles];
	uint8_t viden;
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

#define num_of_enemys 4

//SEZNAM SLIKIC
extern sprite sp[num_of_enemys];
extern sprite sp_original[num_of_enemys];

//DEFINICIJA MAPE
#define map_width 50
#define map_height 25
extern uint8_t map[map_height][map_width];

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

//DEFINICIJE MEGLE
#define render_distance 15
#define density 0.0326
#define gradient 5

extern int g_scaling;

extern stopwatch_handle_t score;
extern float prevScore;

extern int sound_counter;
extern int sound_limit;
extern int sound_select;

extern const uint8_t sample1[3209];
extern const uint8_t sample2[2984];
extern const uint8_t sample3[3721];
extern const uint8_t sample4[9330];

extern uint8_t firstInit;

extern const uint8_t t_floor[3072];
extern const uint8_t t_wall[3072];

extern uint8_t max_time_gun;
extern uint8_t time_gun;
extern uint8_t gun_button_prev;

#define player_speed 2
#define player_turning_speed 0.08

#endif /* INCLUDE_PODATKI_H_ */
