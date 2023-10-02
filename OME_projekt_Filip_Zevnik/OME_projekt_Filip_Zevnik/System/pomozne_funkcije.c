/*
 * pomozne_funkcije.c
 *
 *  Created on: May 16, 2022
 *      Author: zevni
 */

#include "pomozne_funkcije.h"
#include "podatki.h"
#include "kbd.h"
#include "SCI.h"
#include "LED.h"
#include "stdint.h"


void narisi_velik_kvadrat(int x, int y, __uint16_t c)
{
	pixels[(y)*240 + (x)] = c;
	pixels[(y)*240 + (x+1)] = c;
	pixels[(y)*240 + (x+2)] = c;

	pixels[(y+1)*240 + (x)] = c;
	pixels[(y+1)*240 + (x+1)] = c;
	pixels[(y+1)*240 + (x+2)] = c;

	pixels[(y+2)*240 + (x)] = c;
	pixels[(y+2)*240 + (x+1)] = c;
	pixels[(y+2)*240 + (x)+2] = c;

}
void narisi_velik_kvadrat_stene(int x, int y, __uint16_t c)
{
	pixels[(y)*240 + (x)] = c;
	pixels[(y)*240 + (x+1)] = c;
	pixels[(y)*240 + (x+2)] = c;
	pixels[(y)*240 + (x+3)] = c;
	pixels[(y)*240 + (x+4)] = c;

	pixels[(y+1)*240 + (x)] = c;
	pixels[(y+1)*240 + (x+1)] = c;
	pixels[(y+1)*240 + (x+2)] = c;
	pixels[(y+1)*240 + (x+3)] = c;
	pixels[(y+1)*240 + (x+4)] = c;

	pixels[(y+2)*240 + (x)] = c;
	pixels[(y+2)*240 + (x+1)] = c;
	pixels[(y+2)*240 + (x)+2] = c;
	pixels[(y+2)*240 + (x)+3] = c;
	pixels[(y+2)*240 + (x)+4] = c;
}
void reset() {
	map[6][2] = 3;

	sp[0].tip = 0; sp[0].map = 0; sp[0].x = 1.5 * Blocksize; sp[0].y = 5 * Blocksize; sp[0].sizeX = 128; sp[0].sizeY = 128; sp[0].life = 3;
	sp[0].damadgedTime = 0; sp[0].damadgedTimeMax = 300000;
	sp[1].tip = 0; sp[1].map = 0; sp[1].x = 2.5 * Blocksize; sp[1].y = 7.5 * Blocksize; sp[1].sizeX = 128; sp[1].sizeY = 128; sp[1].life = 3;
	sp[1].damadgedTime = 0; sp[1].damadgedTimeMax = 300000;

	px = width / 2;
	py = height / 2;
	angle = PI;
}
float popraviKot(float kot) {
	if (kot < 0) return kot + 2 * PI;
	else if (kot > 2 * PI) return kot - 2 * PI;
	return kot;
}
unsigned long create_rgb(int r, int g, int b)
{
	return ((((r>>3)<<11) | ((g>>2)<<5) | (b>>3)));
}

/*int f32_to_q31(float input){
	const float Q31_MAX_F = 0x0.FFFFFFp0F;
	const float Q31_MIX_F = -1.0F;
	return (int)roundf(scalbnf(fmaxf(fminf(input, Q31_MAX_F), Q31_MIX_F), 31));
}
*/

