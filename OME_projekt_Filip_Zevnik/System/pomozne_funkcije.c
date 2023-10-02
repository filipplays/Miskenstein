/*
 * pomozne_funkcije.c
 *
 *  Created on: May 16, 2022
 *      Author: zevni
 */
#include "timing_utils.h"
#include "periodic_services.h"
#include "lcd.h"
#include "pomozne_funkcije.h"
#include "podatki.h"
#include "kbd.h"
#include "SCI.h"
#include "LED.h"
#include "stdint.h"
#include "stdlib.h"
#include "map.h"
//#include "cover_photo_complete.ppm"
#include "cover_photo_2.ppm"
//#include "victory_text.ppm"
#include "victory_text_small.ppm"
#include "map.h"
#include <math.h>
#include <stdio.h>


void narisi_velik_kvadrat(int x, int y, __uint16_t c)
{
	int ay = y*240;
	if(ay+x+2 < 57600){
		pixels[ay + (x)] = c;
		pixels[ay + (x+1)] = c;
		pixels[ay + (x+2)] = c;
	}

	ay = (y+1)*240;
	if(ay+x+2 < 57600){
		pixels[ay + (x)] = c;
		pixels[ay + (x+1)] = c;
		pixels[ay + (x+2)] = c;
	}

	ay = (y+2) * 240;
	if(ay+x+2 < 57600){
		pixels[ay + (x)] = c;
		pixels[ay + (x+1)] = c;
		pixels[ay + (x)+2] = c;
	}

}
void narisi_velik_kvadrat_stene(int x, int y, __uint16_t c)
{
	int ay;
	ay = y*240;
	if(ay+x+4 < 57600){
		pixels[ay + (x)] = c;
		pixels[ay + (x+1)] = c;
		pixels[ay + (x+2)] = c;
		pixels[ay + (x+3)] = c;
		pixels[ay + (x+4)] = c;
	}

	ay = (y+1)*240;
	if(ay + x + 4 < 57600){
		pixels[ay + (x)] = c;
		pixels[ay + (x+1)] = c;
		pixels[ay + (x+2)] = c;
		pixels[ay + (x+3)] = c;
		pixels[ay + (x+4)] = c;
	}

	ay = (y+2) * 240;
	if(ay + x + 4 < 57600){
		pixels[ay + (x)] = c;
		pixels[ay + (x+1)] = c;
		pixels[ay + (x)+2] = c;
		pixels[ay + (x)+3] = c;
		pixels[ay + (x)+4] = c;
	}
}
void reset() {
	//map[6][2] = 3;

	/*sp[0].tip = 0; sp[0].map = 0; sp[0].x = 1.5 * Blocksize; sp[0].y = 5 * Blocksize; sp[0].sizeX = 128; sp[0].sizeY = 128; sp[0].life = 3;
	sp[0].damadgedTime = 0; sp[0].damadgedTimeMax = 25;*/
	/*p[1].tip = 0; sp[1].map = 0; sp[1].x = 2.5 * Blocksize; sp[1].y = 7.5 * Blocksize; sp[1].sizeX = 128; sp[1].sizeY = 128; sp[1].life = 3;
	sp[1].damadgedTime = 0; sp[1].damadgedTimeMax = 300000;*/

	/*px = width / 2;
	py = height / 2;
	angle = PI;*/
	LEDs_off(255);
	uint16_t x, y;
	int counter = 0;
	stopwatch_handle_t sw_reset;
	UG_COLOR red = create_rgb(255, 0, 0);
	while (counter < 700) {
		x = rand() % 233 + 40;
		y = rand() % 233;
//		UG_FillCircle(x, y, 7, red);
		UG_FillFrame(x, y, x+8, y+8, red);
		TIMUT_stopwatch_set_time_mark(&sw_reset);
		while(!TIMUT_stopwatch_has_X_ms_passed(&sw_reset, 1));

		counter++;
	}
}

void init(){
	UG_SetBackcolor( C_BLACK ) ;
	UG_SetForecolor( C_RED ) ;
//	UG_FillScreen(C_BLACK);
	UG_FillFrame(40, 0, 280, 240, C_BLACK);
	if(firstInit == 1){
		firstInit = 0;
		draw_full_screen_image(t_cover_photo_complete);
		uint32_t seed;
		KBD_scan();
		while(KBD_get_button_state(BTN_OK) != BTN_SIG_VALUE_PRESSED){
			KBD_scan();
		}
		seed = HAL_GetTick();
		srand(seed);
		generateMap();
		create_level_one_sprites();
		LEDs_on(240);
		copy_sprites();
		px = 20*Blocksize;
		py = 12*Blocksize;
		angle = PI/4;
		TIM5->CCR2 = 0;
		TIMUT_stopwatch_set_time_mark(&score);
	}else{
		draw_full_screen_image(t_cover_photo_complete);
		KBD_scan();
		while(KBD_get_button_state(BTN_OK) != BTN_SIG_VALUE_PRESSED){
			KBD_scan();
		}
		LEDs_on(240);
		copy_sprites();
		px = 20*Blocksize;
		py = 12*Blocksize;
		angle = PI/4;
		TIM5->CCR2 = 0;
		TIMUT_stopwatch_set_time_mark(&score);
	}
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

//FUNKCIJA ZA RACUNANJE VIDLJIVOSTI
float visibility(float distance)
{
    return exp(-pow(distance * density, gradient));
}

float map_range(float min1, float max1, float min2, float max2, float value)
{
	return min2 + (max2 - min2) * ((value - min1) / (max1 - min1));
}
void shoot()
{
	if(time_gun == 0){
		KBD_scan();
		if(KBD_get_button_state(BTN_OK) == BTN_STATE_RELEASED && gun_button_prev == BTN_STATE_PRESSED){
			time_gun = max_time_gun;
			play_sound(3);
			UG_FillFrame(40, 0, 240, 240,create_rgb(233, 235, 162));
			for(int i = 0; i < num_of_enemys;i++)
			{
				if(sp[i].tip == 1 && sp[i].life > 0 && sp[i].damadgedTime == 0)
				{
					play_sound(0);
					sp[i].damadgedTime = sp[i].damadgedTimeMax;
					sp[i].life--;
					if(sp[i].life < 1){
						check_victory();
						uint8_t check = 240;
						LEDs_off(240);
						for(int j=0;j<num_of_enemys;j++){
							if(sp[j].life < 1) check = check << 1;
						}
						LEDs_on(check);
					}
					initBlood(sp[i].blood, 0, 0, sp[i].dist);
				}
			}
		}
	}
	gun_button_prev = KBD_get_button_state(BTN_OK);
}

void moveBlood(Blood *blood) {
	blood->posx += blood->velx;
	blood->posy += blood->vely;
	blood->velx *= friction;
	blood->vely *= friction;
	blood->vely += gravity;
	if (blood->velx > maxspeed) blood->velx = maxspeed;
	if (blood->vely > maxspeed) blood->vely = maxspeed;
}

void initBlood(Blood blood[], uint8_t x, uint8_t y, float dist) {
    float scale = 1 / (dist*0.05);
    if(scale > 1) scale = 1;
    for (int i = 0;i < num_of_blood_particles;i++) {
        blood[i].posx = x;
        blood[i].posy = y;
        int range = 100;
        blood[i].velx = (((double)(rand() % (12 * range)) / (range)) - 6)*scale;
        blood[i].vely = (-(double)(rand() % (5 * range)) / (range)) * scale;
        blood[i].active = 1;
    }
}

void drawBlood(Blood blood, float sx, float sy, float dist) {
	float realx = blood.posx + sx;
	float realy = blood.posy + sy;
	float vis = visibility(dist);
	//219, 43, 18
	UG_COLOR c = create_rgb(255*vis, 0, 0);
	if(realx < 40 || realx > 280 || realy < 0 || realy > 240) return;
	narisi_velik_kvadrat(realx,realy, c);
    //glColor3ub(237, 126, 109);
    //glPointSize(25*scale);
	//glBegin(GL_POINTS);
	//glVertex2f(blood.posx + sx,blood.posy + sy);
	//glEnd();
}

/*int f32_to_q31(float input){
	const float Q31_MAX_F = 0x0.FFFFFFp0F;
	const float Q31_MIX_F = -1.0F;
	return (int)roundf(scalbnf(fmaxf(fminf(input, Q31_MAX_F), Q31_MIX_F), 31));
}
*/

void draw_full_screen_image(const uint8_t *img){
	int pixel = 0;
	uint8_t red, green, blue;
	for(int y=0;y<240;y++){
		for(int x=0;x<240;x++){
			pixel = ((y*240) + x) * 3;
			red = img[pixel + 0];
			green = img[pixel + 1];
			blue = img[pixel + 2];
			pixels[y*240 + x] = create_rgb(red, green, blue);
		}
	}
	ILI9341_SetDisplayWindow(40, 0, 240, 240);
	ILI9341_SendData(pixels, 57600);
}

void draw_full_screen_image_without_red(const uint8_t *img){
	int pixel = 0;
	uint8_t red, green, blue;
	//int offset = 55*240;
	for(int y=0;y<130;y++){
		for(int x=0;x<240;x++){
			pixel = ((y*240) + x) * 3;
			red = img[pixel + 0];
			green = img[pixel + 1];
			blue = img[pixel + 2];
			if(red != 255 || green != 255 || blue != 255)
				pixels[(y+55)*240 + x] = create_rgb(red, green, blue);
		}
	}
	ILI9341_SetDisplayWindow(40, 0, 240, 240);
	ILI9341_SendData(pixels, 57600);
}

void check_victory(){
	for(int i=0;i<num_of_enemys;i++){
		if(sp[i].life > 0) return;
	}
	TIMUT_stopwatch_update(&score);
	play_sound(1);
	float curr_score = (float)score.elapsed_time / 1000.0;
	draw_full_screen_image_without_red(t_victory_screen);
	stopwatch_handle_t victory;
	TIMUT_stopwatch_set_time_mark(&victory);
	while(!TIMUT_stopwatch_has_X_ms_passed(&victory, 5000)){
		LEDs_on(255);
		HAL_Delay(1000);
		LEDs_off(255);
		HAL_Delay(1000);
	}
	handle_score(curr_score);
	reset();
	init();
}

void create_level_one_sprites(){
	int tmpx, tmpy, done = 0;
	for(int i=0;i<num_of_enemys;i++){
		sp_original[i].sizeX = 128;
		sp_original[i].sizeY = 128;
		sp_original[i].life = 3;
		sp_original[i].damadgedTime = 0;
		sp_original[i].damadgedTimeMax = 5;
		sp_original[i].tip = 0;
		sp_original[i].map = 0;
		sp_original[i].viden = 0;
		done = 0;
		while(1){
			tmpx = rand()%23+25;
			tmpy = rand()%22+1;
			if(map[tmpy][tmpx] == 0){
				done = 1;
				for(int i=0;i<num_of_enemys;i++){
					if((tmpx+0.5) * Blocksize == sp_original[i].x && (tmpy+0.5) * Blocksize == sp_original[i].y) done = 0;
				}
			}
			if(done == 1) break;
		}
		sp_original[i].x = (tmpx+0.5) * Blocksize;
		sp_original[i].y = (tmpy+0.5) * Blocksize;
	}

}

void copy_sprites(){
	for(int i=0;i<num_of_enemys;i++){
		memcpy(&sp[i], &sp_original[i], sizeof(sprite));
	}
}

void handle_score(float currScore){
	char str1[70];
	char str2[70];
	if(prevScore == 0 || prevScore > currScore){
		sprintf(str1, "New High Score: %.3f", currScore);
		sprintf(str2, "Last Score: %.3f", prevScore);
		prevScore = currScore;
	}
	else{
		sprintf(str1, "The High Score Is: %.3f", prevScore);
		sprintf(str2, "Current Score: %.3f", currScore);
	}

	float delta = 32.0 / 240.0;
	uint8_t red, green, blue;
	int pixel = 0;
	int countX = 0;
	int countY = 0;
	for(float y=0;y<32;y+=delta){
		countX = 0;
		for(float x=0;x<32;x+=delta){
			pixel = ((int)(y)*32+(int)x)*3;
			if(pixel <= 32*32*3-3 && countY*240+countX < 57600){
				red = t_wall[pixel + 0];
				green = t_wall[pixel + 1];
				blue = t_wall[pixel + 2];

				pixels[countY*240+countX] = create_rgb(red, green, blue);
			}
			countX++;
		}
		countY++;
	}
	UG_FillFrame(40, 0, 280, 240, C_BLACK);
	ILI9341_SetDisplayWindow(40, 0, 240, 240);
	ILI9341_SendData(pixels, 57600);

	UG_FillFrame(40, s_height/2-40, 280, s_height/2-40+12, create_rgb(0, 0, 0));
	UG_FillFrame(40, s_height/2-28, 280, s_height/2-28+12, create_rgb(0, 0, 0));
	UG_FillFrame(40, s_height/2+25, 280, s_height/2+25+12, create_rgb(0, 0, 0));
	UG_FillFrame(40, s_height/2+45, 280, s_height/2+45+12, create_rgb(0, 0, 0));

	uint16_t size1 = strlen(str1) * 9;
	uint16_t size2 = strlen(str2) * 9;

	UG_SetForecolor(create_rgb(255, 112, 112));
	UG_PutString(s_width/2-size1/2, s_height/2-40, str1);
	UG_PutString(s_width/2-size2/2, s_height/2-28, str2);

	UG_SetForecolor(create_rgb(112, 122, 255));
	UG_PutString(s_width/2-24*4.5, s_height/2+25, "Press OK to continue! ->");
	UG_PutString(s_width/2-26*4.5, s_height/2+45, "Press RESET for new level!");



	KBD_scan();
	while(KBD_get_button_state(BTN_OK) != BTN_SIG_VALUE_PRESSED){
		KBD_scan();
	}

}

void play_sound(uint8_t sound){
	switch(sound){
	case 0:
		sound_select = 0;
		sound_counter = 0;
		sound_limit = 3209;
		break;
	case 1:
		sound_select = 1;
		sound_counter = 0;
		sound_limit = 2984;
		break;
	case 2:
		sound_select = 2;
		sound_counter = 0;
		sound_limit = 3721;
		break;
	case 3:
		sound_select = 3;
		sound_counter = 0;
		sound_limit = 9330;
	default:
		return;
	}
}
