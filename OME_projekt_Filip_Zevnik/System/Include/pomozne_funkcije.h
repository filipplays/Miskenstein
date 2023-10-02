/*
 * pomozne_funkcije.h
 *
 *  Created on: May 16, 2022
 *      Author: zevni
 */

#ifndef INCLUDE_POMOZNE_FUNKCIJE_H_
#define INCLUDE_POMOZNE_FUNKCIJE_H_

#include "stdint.h"
#include "podatki.h"

void narisi_velik_kvadrat(int, int, uint16_t);
void narisi_velik_kvadrat_stene(int, int, uint16_t);
void reset();
float popraviKot(float);
unsigned long create_rgb(int, int, int);
//FUNKCIJA ZA RACUNANJE VIDLJIVOSTI
float visibility(float distance);
void shoot();

float map_range(float min1, float max1, float min2, float max2, float value);

//ZA KRI
void moveBlood(Blood blood[]);

void initBlood(Blood blood[], uint8_t x, uint8_t y, float dist);

inline void bloodOutOfBounds(Blood* blood, float maxy) {
    if (blood->posy > maxy) {
		blood->active = 0;
	}
}

void drawBlood(Blood blood, float sx, float sy, float scale);

void draw_full_screen_image(const uint8_t *img);

void init();

void draw_full_screen_image_without_red(const uint8_t *img);

void create_level_one_sprites();

void check_victory();

void handle_score(float currScore);

void copy_sprites();

void fix_bug();

uint8_t check_enemy();

void play_sound(uint8_t);

#endif /* INCLUDE_POMOZNE_FUNKCIJE_H_ */
