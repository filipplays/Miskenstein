/*
 * ray_funkcije.c
 *
 *  Created on: May 16, 2022
 *      Author: zevni
 */
#include "podatki.h"
#include "pomozne_funkcije.h"
#include "ray_funkcije.h"
#include "kbd.h"
#include "LED.h"
#include "stdint.h"
#include "math.h"
#include "joystick.h"
#include "timing_utils.h"

#include "stm32g4xx.h"

#include "t_gun.ppm"
#include "t_brickWall.ppm"
#include "t_gun_2.ppm"
//#include "t_floor.ppm"
//#include "t_wood.ppm"

#include "t_enemy.ppm"

#include <stdio.h>
#include <string.h>

stopwatch_handle_t sw1;
stopwatch_handle_t sw2;

float sprite_x;
uint8_t sprite_draw;
float sprite_angle_diff;

void drawGun() {
	float scale = 1.0f / 2.0f;
	int tx_step = 1.0f / scale;
	int ty_step = 1.0f / scale;
	int red, green, blue;
	for (int y = 0; y < 128; y += ty_step) {
		for (int x = 0; x < 128; x += tx_step) {
			int pixel = ((y) * 128 + x) * 3;
			if(time_gun == 0){
				red = t_guns[pixel + 0];
				green = t_guns[pixel + 1];
				blue = t_guns[pixel + 2];
			}else{
				red = t_gun_2[pixel + 0];
				green = t_gun_2[pixel + 1];
				blue = t_gun_2[pixel + 2];
			}
			if ((red + green + blue) < 689) {
				int gun_x = width / 2 - 128 * scale / 2
						+ (int) ((float) x / tx_step);
				int gun_y = height - 128 * scale + (int) ((float) y / ty_step)
						+ 8;

				gun_x *= s_scale;
				gun_x += s_offset;
				gun_y *= s_scale;

				narisi_velik_kvadrat(gun_x, gun_y,
						create_rgb(red, green, blue));
			}
		}
	}
	if(time_gun > 0) time_gun--;
}
void drawMap(CORDIC_HandleTypeDef *hc) {
	TIMUT_stopwatch_set_time_mark(&sw1);
	float dx = 0, dy = 0, d1 = 10000, d2 = 10000, rvx = 0, rvy = 0, dof = 0,
			newAngle = 0, rhx = 0, rhy = 0, d = 0, rx = 0, ry = 0;
	int wx = 0, wy = 0;
	float vis;
	KBD_scan();

	int ipx = (int) px;
	int ipy = (int) py;

	//MOVE PLAYER
	if (JOY_get_axis_position(Y) < 40) {
		if (JOY_get_axis_position(X) < 40)
			angle += player_turning_speed;
		if (JOY_get_axis_position(X) > 60)
			angle -= player_turning_speed;
	} else {
		if (JOY_get_axis_position(X) < 40)
			angle -= player_turning_speed;
		if (JOY_get_axis_position(X) > 60)
			angle += player_turning_speed;
	}
	if (angle < 0)
		angle = 2 * PI;
	else if (angle > 2 * PI)
		angle = 0;

	//POGLEJ A SE PLAYER LAHKO PREMAKNE
	int fCheck = 5;
	int fCheckX = 0, fCheckY = 0;
	if (cosf(angle) < 0)
		fCheckX = -fCheck;
	else
		fCheckX = fCheck;
	if (sinf(angle) < 0)
		fCheckY = -fCheck;
	else
		fCheckY = fCheck;

	if (JOY_get_axis_position(Y) > 60) {
		if (map[ipy / 10][(ipx + fCheckX) / 10] == 0)
			px += player_speed * cosf(angle);
		if (map[(ipy + fCheckY) / 10][ipx / 10] == 0)
			py += player_speed * sinf(angle);
	}
	if (JOY_get_axis_position(Y) < 40) {
		if (map[ipy / 10][(ipx - fCheckX) / 10] == 0)
			px += -player_speed * cosf(angle);
		if (map[(ipy - fCheckY) / 10][ipx / 10] == 0)
			py += -player_speed * sinf(angle);
	}


	//DA LAHKO IGRALEC ODPRE VRATA
	float roka = 6;
	float rokaPosX = px + roka * cosf(angle);
	float rokaPosY = py + roka * sinf(angle);
	if (!KBD_get_button_state(BTN_ESC)
			&& map[(int) rokaPosY / Blocksize][(int) rokaPosX / Blocksize] == 3)
		map[(int) rokaPosY / Blocksize][(int) rokaPosX / Blocksize] = 0;

	for (float z = -30; z < 30; z += 1) { //LOOPEJ OD - 30STOPINJ DO 30 STOPINJ
		int vmt = 0, hmt = 0; //HORIZONTALNA IN VERTIKALNA ŠTEVILKA STENE DA VEM KATERO TEKSTURO NARISAT
		float newAngle = z * DEGREE + angle;
		if (newAngle < 0)
			newAngle += 2 * PI;
		else if (newAngle > 2 * PI)
			newAngle -= 2 * PI;

		//GLEJ GOR PA DOL
		float aTan = 1 / tanf(newAngle);
		dof = 0;
		if (newAngle >= PI) { //GLEDA GOR
			rvy = ipy - (ipy % Blocksize) - 0.0001;
			rvx = (py - rvy) * -aTan + px;
			dy = -Blocksize;
			dx = dy * aTan;
		} else if (newAngle < PI) {  //GLEDA DOL
			rvy = ipy - (ipy % Blocksize) + Blocksize;
			rvx = (py - rvy) * -aTan + px;
			dy = Blocksize;
			dx = dy * aTan;
		} else if (newAngle == 0 || newAngle == PI) { // GLEDA  CIST LECVO ALPA CIST DESNO
			rvx = px;
			rvy = py;
			dof = render_distance;
		}

		//uint8_t mapy = sizeof(map) / sizeof(map[0]);

		while (dof < render_distance) { //PREVER ALI SE KAM ZALETI V "render_distance" KORAKIH
			wx = rvx / Blocksize;
			wy = rvy / Blocksize;
			if (wx > -1 && wx < map_width && wy > -1 && wy < map_height && map[(int) wy][(int) wx] != 0) {
				dof = render_distance;
				vmt = map[(int) wy][(int) wx];
			} else {
				rvx += dx;
				rvy += dy;
				dof++;
			}
		}

		d1 = sqrt((px - rvx) * (px - rvx) + (py - rvy) * (py - rvy));
//		d1 = (ipy - (ipy % Blocksize) - 0.0001 + dof*Blocksize) / sinf(newAngle);

		//GLEJ LEVO PA DESNO
		dof = 0;
		float nTan = tanf(newAngle);
		if (newAngle >= ((3 * PI) / 2) || newAngle <= PI / 2) { //GLEDA DESNO
			rhx = ipx - (ipx % Blocksize) + Blocksize;
			rhy = (px - rhx) * -nTan + py;
			dx = Blocksize;
			dy = dx * nTan;
		} else if (newAngle > PI / 2 && newAngle < (3 * PI) / 2) {  //GLEDA LEVO
			rhx = ipx - (ipx % Blocksize) - 0.0001;
			rhy = (px - rhx) * -nTan + py;
			dx = -Blocksize;
			dy = dx * nTan;
		} else if (newAngle == PI / 2 || newAngle == (3 * PI) / 2) {
			rhx = px;
			rhy = py;
			dof = render_distance;
		}
		while (dof < render_distance) { //PREVERI ALI SI SE ZALETU V 15 KORAKIH
			wx = rhx / Blocksize;
			wy = rhy / Blocksize;
			if (wx > -1 && wx < map_width && wy > -1 && wy < map_height
					&& map[wy][wx] != 0) {
				dof = render_distance;
				hmt = map[wy][wx];
			} else {
				rhx += dx;
				rhy += dy;
				dof++;
			}
		}
		d2 = sqrt((px - rhx) * (px - rhx) + (py - rhy) * (py - rhy));
//		d2 = (ipx - (ipx % Blocksize) + Blocksize + dof) * dy;

		//NARIŠI ZARKE IN 3D MAPO
		float shade = 1;
		//glLineWidth(2);
		if (d1 > d2) {
			//glColor3f(1, 1, 1);
			//line(px, py, rhx, rhy);
			d = d2;
			shade = 0.5;
			rx = rhx;
			ry = rhy;
		} else {
			//glColor3f(255 / 255, 109 / 255, 119 / 255);
			//line(px, py, rvx, rvy);
			d = d1;
			rx = rvx;
			ry = rvy;
			hmt = vmt; //DA SE OBARVAJO VSE STRANI
		}
		vis = visibility(d);
		//NARIŠI STENE
		float lineH = (Blocksize * 100) / (d * cosf(z * DEGREE)); //POPRAVI FISHEYE
		float ty_step = 32.0f / (float) lineH; //ZATO DA SE TEKSTURA ENAKOMERNO PORAZDELI
		float ty_offset = 0; //ZATO DA SE TEKSTURE LEPO PORAZDELIJO
		if (lineH > height) {
			ty_offset = (lineH - height) / 2.0f;
			lineH = height;
		}
		float lineOff = 50 - lineH / 2;
		float ty = ty_offset * ty_step; //+ (hmt - 1) * 32;
		float scale = (float) Blocksize / 32.0f;
		float tx = 0;
		if (shade == 1) {
			tx = (int) (rx / scale) % 32;
			if (newAngle < 2 * PI)
				tx = 31 - tx;
		} else {
			tx = (int) (ry / scale) % 32;
			if (newAngle > PI / 2 && newAngle < (3 * PI) / 2)
				tx = 31 - tx;
		}

		//glPointSize(25);
		//glBegin(GL_POINTS);
		int wallx = (int) ((float) (z + 30) * width / 60);
		wallx *= s_scale;
		wallx += s_offset;

		for (int j = 0; j < lineH; j++) {

			int pixel = ((int) ty * 32 + (int) tx) * 3;
			//cout << pixel << endl;
			int red = 0, blue = 0, green = 0;
			switch (hmt) {
			case 1:
				red = t_brickWall[pixel + 0] * shade * vis;
				green = t_brickWall[pixel + 1] * shade * vis;
				blue = t_brickWall[pixel + 2] * shade * vis;
				break;
			case 2:
				red = t_wall[pixel + 0] * shade * vis;
				green = t_wall[pixel + 1] * shade * vis;
				blue = t_wall[pixel + 2] * shade * vis;
				break;
			case 3:
				/*red = t_wood[pixel + 0] * shade * vis;
				green = t_wood[pixel + 1] * shade * vis;
				blue = t_wood[pixel + 2] * shade * vis;*/
				break;

			default:
				break;
			}
			int wally = lineOff + j;
			wally *= s_scale;

			narisi_velik_kvadrat_stene(wallx, wally,
					create_rgb(red, green, blue));
			ty += ty_step;
		}

		//glEnd();

		//NARIŠI TLA IN STROP
		//glPointSize(25);
		//glBegin(GL_POINTS);
		int stropx = (int) ((float) ((z + 30) * width) / 60);
		stropx *= s_scale;
		stropx += s_offset;
		int tlax = (int) ((float) (z + 30) * width / 60);
		tlax *= s_scale;
		tlax += s_offset;
		float floorAngle = (angle - newAngle);
		if (floorAngle < 0)
			floorAngle += 2 * PI;
		else if (floorAngle > 2 * PI)
			floorAngle -= 2 * PI;

		float tmpAngle;
		float cosNew, sinNew, cosFloor, sinFloor;

		/*CORDIC_ConfigTypeDef cordic_config;
		 cordic_config.OutSize = CORDIC_OUTSIZE_32BITS;
		 cordic_config.InSize = CORDIC_OUTSIZE_32BITS;
		 cordic_config.Scale = CORDIC_SCALE_0;
		 cordic_config.Function = CORDIC_FUNCTION_COSINE;*/

		cosNew = cosf(newAngle);
		cosFloor = cosf(floorAngle);
		sinNew = sinf(newAngle);

		float tmptx2 = cosf(newAngle) * 1847.52;
		float tmpty2 = sinf(newAngle) * 1847.52;
		float tmptx1 = px / 0.3;
		float tmpty1 = py / 0.3;

		for (int y = lineOff + lineH; y < height; y++) {
			vis = 1;
			float dy = y - (height / 2.0f);

			/*//calculate the sin and cos values
			 //cordic_config.Function = CORDIC_FUNCTION_COSINE;
			 HAL_CORDIC_Configure(hc, &cordic_config);
			 if(newAngle > PI) tmpAngle = newAngle - 2*PI;
			 int32_t input = (tmpAngle / PI) * (1<<31);
			 int32_t result;
			 HAL_StatusTypeDef cordic_status = HAL_CORDIC_Calculate(hc, &input, &result, 1, HAL_MAX_DELAY);
			 cosNew = -1*((float)result / (float)(1 << 31));
			 cordic_cos = cosNew;
			 math_cos = cosf(newAngle);
			 sum = fabs(cordic_cos) + fabs(math_cos);*/

			tx = tmptx1 + tmptx2 / dy / cosFloor;
			ty = tmpty1 + tmpty2 / dy / cosFloor;
			int pixel = (((int) (ty) & 31) * 32 + ((int) (tx) & 31)) * 3;

			if (y - height / 2 < 13) {
				float dist = 30;
				vis = visibility(dist);
			}

			int red = t_floor[pixel + 0] * vis /** 0.7*/;
			int green = t_floor[pixel + 1] * vis /** 0.7*/;
			int blue = t_floor[pixel + 2] * vis /** 0.7*/;

			int tlay = y;
			tlay *= s_scale;

			narisi_velik_kvadrat_stene(tlax, tlay,
					create_rgb(red, green, blue));

			//NARISI STROP
			red = t_wall[pixel + 0] * vis /** 0.7*/;
			green = t_wall[pixel + 1] * vis /** 0.7*/;
			blue = t_wall[pixel + 2] * vis /** 0.7*/;

			int stropy = height - y;
			stropy *= s_scale;

			narisi_velik_kvadrat_stene(stropx, stropy,
					create_rgb(red, green, blue));
		}

	}
	TIMUT_stopwatch_update(&sw1);
}

void drawSprites() {
	//IZRACUNAJ ODDALJENOSTI OD IGRALCA
	for (int i = 0; i < num_of_enemys; i++) {
		sp[i].dist = sqrtl(
				(px - sp[i].x) * (px - sp[i].x)
						+ (py - sp[i].y) * (py - sp[i].y));
	}

	//RAZPOREDI SPRAJTE PO RAZDALJI -> MEHURCEK
	sprite tmp;
	for (int i = 0; i < num_of_enemys - 1; i++) {
		for (int j = 0; j < num_of_enemys - i - 1; j++) {
			if (sp[j].dist <= sp[j + 1].dist) {
				memcpy(&tmp, &sp[j], sizeof(sprite));
				memcpy(&sp[j], &sp[j+1], sizeof(sprite));
				memcpy(&sp[j+1], &tmp, sizeof(sprite));
			}
		}
	}

	uint8_t indicate_led = 0;
	//POJDI SKOZE VSE SLIKICE
	for (int sprite = 0; sprite < num_of_enemys; sprite++) {

		uint8_t draw = 1;
		//POZICIJA
		float sx = sp[sprite].x;
		float sy = sp[sprite].y;
		//TX IN TY STA POZICIJI V TABELI Z RGB VREDNOSTMI IN STEP SO KORAKI KI SE SPREMINJAJO Z VELIKOSTJO SLIKICE
		float tx = 0, ty = 0, tx_step = 0, ty_step = 0;
		int pixel = 0;
		uint8_t red = 0, green = 0, blue = 0;

		//IZRACUNAJ KOT SLIKICE GLEDE NA IGRALCA
		float sangle = atanf((sy - py) / (sx - px));
		//TO NEVEM KAJ NARDI
		if (fabs(px + cosf(sangle) - sx) > fabs(px - sx)) {
			sangle = popraviKot((sangle - PI));
		}

		float angleDiff = popraviKot(angle - sangle);
		sprite_angle_diff = angleDiff;
		//POGLEJ CE SE SLIKICO VIDI
		float dist = sp[sprite].dist;
		if ((angleDiff > 11 * PI / 6 || angleDiff < PI / 6)
				&& dist / Blocksize < render_distance) {

			//TLE POGLEJ A JE KSNA STENA UMES
			for (uint8_t d = 0; d < dist; d += 1) {
				if (map[((int) (py + sinf(sangle) * d) / Blocksize)][((int) (px
						+ cosf(sangle) * d) / Blocksize)] != 0) {
					draw = 0;
					break;
				}
			}

			//POGLEJ CE MA DOST LAJFA
			if (sp[sprite].life < 1)
				draw = 0;

			//PRIMERNO SPREMENI SLIKICO GLEDE NA ODDALJENOST IN JO NARISI
			float factor = dist * cosf(angleDiff);
			if(factor <= 0.2) factor = 0.2;
			float scaling = 15 * 1 / (factor);
			if (scaling > 7) scaling = 7;
			else if(scaling <= 0) scaling = 0.2;
			//SX IN SY STA POLOZAJA SLIKICE NA EKRANU
			float sy = s_height / 2 - scaling * sp[sprite].sizeY / 2;
			if (angleDiff > PI / 6)
				angleDiff -= 2 * PI;
			float sx = (map_range(-PI / 6, PI / 6, PI / 3, 0, angleDiff)
					/ (PI / 3)) * s_height - scaling * sp[sprite].sizeX / 2;
			sprite_x = sx;
			tx_step = (1.0 / scaling);
			ty_step = (1.0 / scaling);
			sprite_draw = draw;
			//POGLEJ ALI SE GA DA USTRELT IN SPREMENI TIP
			if (sx < s_height / 2
				&& sx + sp[sprite].sizeX * scaling > s_height / 2
				&& draw == 1 && sp[sprite].damadgedTime < 1) {
//				if(indicate_led == 0){
					sp[sprite].tip = 1;
//				}else{
//					sp[sprite].tip = 0;
//				}
				indicate_led = 1;
			}else{
				sp[sprite].tip = 0;
			}
			if (draw == 1) {
				sp[sprite].viden = 1;
				float vis = visibility(dist);
				float add_red = 75;
				//float y = 0, x = 0;
				int countx = 0, county = 0;
				for (float y = 0; y < sp[sprite].sizeY; y += ty_step) {
					countx = 0;
					for (float x = 0; x < sp[sprite].sizeX; x += tx_step) {
						pixel = ((int) y * sp[sprite].sizeY + (int) x) * 3;
						red = t_enemy[pixel + 0];
						green = t_enemy[pixel + 1];
						blue = t_enemy[pixel + 2];
						/*if(check_enemy() == 1){
							HAL_Delay(1);
						}*/
						if ((red != 188 || blue != 0 || green != 0)) {
							if (sp[sprite].damadgedTime > 0) {
								if(red + add_red > 255) red = 255;
								else red += add_red;
							}
							red *= vis;
							green *= vis;
							blue *= vis;
							//if (sx + x / tx_step < s_height && sx + x / tx_step
								//> 0&& sy + y / ty_step > 0 && sy + y / ty_step < s_height)
								//narisi_velik_kvadrat_stene(sx + x / tx_step, sy + y / ty_step, create_rgb(red, green, blue));
							if (sx + countx < s_height && sx + countx
															> 0&& sy + county > 0 && sy + county < s_height)
								narisi_velik_kvadrat(sx + countx, sy + county, create_rgb(red, green, blue));
						}
						countx++;
					}
					county++;
				}
				//DRAW blood
				if (sp[sprite].damadgedTime > 0) {
					float startx = sx + scaling * sp[sprite].sizeX / 2;
					float starty = sy + scaling * sp[sprite].sizeY / 2;
					for (int i = 0; i < num_of_blood_particles; i++) {
						if (sp[sprite].blood[i].active != 1) continue;
						bloodOutOfBounds(&sp[sprite].blood[i],scaling * sp[sprite].sizeY/2);
						moveBlood(&sp[sprite].blood[i]);
						drawBlood(sp[sprite].blood[i], startx, starty, sp[sprite].dist);
					}
				}
			}
		}

		//TO JE LOGIKA ZA PREMIKANJE TAKO DA DRSI PO STENAH
		if(sp[sprite].viden == 1 && dist > 4){
			int fCheck = 5;
			int fCheckX = 0, fCheckY = 0;
			if (cosf(sangle + PI) < 0)
				fCheckX = -fCheck;
			else
				fCheckX = fCheck;
			if (sinf(sangle + PI) < 0)
				fCheckY = -fCheck;
			else
				fCheckY = fCheck;
			uint8_t mapy = (int) sp[sprite].y / 10;
			uint8_t mapx = ((int) sp[sprite].x + fCheckX) / 10;
			if (map[mapy][mapx] == 0) sp[sprite].x += 0.4 * cosf(sangle + PI);

			mapy = ((int) sp[sprite].y + fCheckY) / 10;
			mapx = (int) sp[sprite].x / 10;
			if (map[mapy][mapx] == 0) sp[sprite].y += 0.4 * sinf(sangle + PI);
		}

		//POGLEJ CE SI SE DOTAKNU IGRALCA
		if (sp[sprite].dist <= 5 && sp[sprite].life > 0) {
			play_sound(2);
			reset();
			init();
			return;
		}
		if(sp[sprite].damadgedTime > 0) sp[sprite].damadgedTime--;
	}
	if(indicate_led){
		LED_on(LED1);
		LED_on(LED2);
	}else{
		LED_off(LED1);
		LED_off(LED2);
	}

}
