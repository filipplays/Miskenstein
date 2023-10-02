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

#include "t_gun.ppm"
#include "t_brickWall.ppm"
#include "t_wall.ppm"
#include "t_wood.ppm"
#include "t_floor.ppm"
#include "hitler_sprite.ppm"


void drawGun() {
	float scale = 1.0f / 2.0f;
	int tx_step = 1.0f / scale;
	int ty_step = 1.0f / scale;
	for (int y = 0;y < 128;y+=ty_step) {
		for (int x = 0;x < 128;x+=tx_step) {
			int pixel = ((y) * 128 + x) * 3;
			int red = t_guns[pixel + 0];
			int green = t_guns[pixel + 1];
			int blue = t_guns[pixel + 2];
			if ((float)(red+green+blue)/765 < 0.9) {
				int gun_x = width/2 - 128*scale/2 + (int)((float)x / tx_step);
				int gun_y = height - 128*scale + (int)((float)y / ty_step) + 8;

				gun_x *= s_scale;
				gun_x += s_offset;
				gun_y *= s_scale;

				narisi_velik_kvadrat(gun_x, gun_y, create_rgb(red, green, blue));
			}
		}
	}
}
void drawMap(CORDIC_HandleTypeDef *hc){

	float dx = 0, dy = 0, d1 = 10000, d2 = 10000, rvx = 0, rvy = 0, dof = 0,  newAngle = 0, rhx = 0, rhy = 0,d = 0, rx= 0, ry=0;
	int wx = 0, wy = 0;
	KBD_scan();

	//MOVE PLAYER
	if(JOY_get_axis_position(Y) < 40)
	{
		if(JOY_get_axis_position(X) < 40) angle += 0.1;
		if(JOY_get_axis_position(X) > 60) angle -= 0.1;
	}else
	{
		if(JOY_get_axis_position(X) < 40) angle -= 0.1;
		if(JOY_get_axis_position(X) > 60) angle += 0.1;
	}
	if (angle < 0) angle = 2 * PI;
	else if (angle > 2 * PI) angle = 0;


	//POGLEJ A SE PLAYER LAHKO PREMAKNE
	int fCheck = 5;
	int fCheckX = 0, fCheckY = 0;
	if (cosf(angle) < 0) fCheckX = -fCheck;
	else fCheckX = fCheck;
	if (sinf(angle) < 0) fCheckY = -fCheck;
	else fCheckY = fCheck;

	if (JOY_get_axis_position(Y) > 60) {
		if(map[(int)py / 10][((int)px + fCheckX) / 10] == 0) px += 3 * cosf(angle);
		if (map[((int)py + fCheckY) / 10][(int)px/ 10] == 0) py += 3 * sinf(angle);
		//LED_on(LED0);
	}else LED_off(LED0);
	if (JOY_get_axis_position(Y) < 40) {
		if (map[(int)py / 10][((int)px - fCheckX) / 10] == 0) px += -3 * cosf(angle);
		if (map[((int)py - fCheckY) / 10][(int)px / 10] == 0) py += -3 * sinf(angle);
		//LED_on(LED0);
	}else LED_off(LED0);

	//DA LAHKO IGRALEC ODPRE VRATA
	float roka = 6;
	float rokaPosX = px + roka * cosf(angle);
	float rokaPosY = py + roka * sinf(angle);
	if (!KBD_get_button_state(BTN_OK) && map[(int)rokaPosY / Blocksize][(int)rokaPosX / Blocksize] == 3) map[(int)rokaPosY / Blocksize][(int)rokaPosX / Blocksize] = 0;

	for (float z = -30;z < 30;z+=1) { //LOOPEJ OD - 30STOPINJ DO 30 STOPINJ
		float vmt = 0, hmt = 0; //HORIZONTALNA IN VERTIKALNA ŠTEVILKA STENE DA VEM KATERO TEKSTURO NARISAT
		float newAngle = z * DEGREE + angle;
		if (newAngle < 0) newAngle += 2 * PI;
		else if (newAngle > 2 * PI) newAngle -= 2 * PI;


		//GLEJ GOR PA DOL
		float aTan = 1 / tanf(newAngle);
		dof = 0;
		if (newAngle >= PI) { //GLEDA GOR
			rvy = (int)py - ((int)py % Blocksize) - 0.0001;
			rvx = (py - rvy) * -aTan + px;
			dy = -Blocksize;
			dx = dy * aTan;
		}
		else if (newAngle < PI) {  //GLEDA DOL
			rvy = (int)py - ((int)py % Blocksize) + Blocksize;
			rvx = (py - rvy) * -aTan + px;
			dy = Blocksize;
			dx = dy * aTan;
		}
		else if (newAngle == 0 || newAngle == PI) { // GLEDA  CIST LECVO ALPA CIST DESNO
			rvx = px;
			rvy = py;
			dof = 15;
		}

		while (dof < 15) { //PREVER ALI SE KAM ZALETI V 15 KORAKIH
			wx = rvx / Blocksize;
			wy = rvy / Blocksize;
			if (wx > -1 && wx < sizeof(map)/sizeof(map[0]) && wy > -1 && wy < sizeof(map)/sizeof(map[0]) && map[(int)wy][(int)wx] != 0) {
				dof = 15;
				vmt = map[(int)wy][(int)wx];
			}
			else {
				rvx += dx;
				rvy += dy;
				dof++;
			}
		}
		d1 = sqrt((px - rvx) * (px - rvx) + (py - rvy) * (py - rvy));


		//GLEJ LEVO PA DESNO
		dof = 0;
		float nTan = tanf(newAngle);
		if (newAngle >= ((3 * PI) / 2) || newAngle <= PI / 2) { //GLEDA DESNO
			rhx = (int)px - ((int)px % Blocksize) + Blocksize;
			rhy = (px - rhx) * -nTan + py;
			dx = Blocksize;
			dy = dx * nTan;
		}
		else if (newAngle > PI / 2 && newAngle < (3 * PI) / 2) {  //GLEDA LEVO
			rhx = (int)px - ((int)px % Blocksize) - 0.0001;
			rhy = (px - rhx) * -nTan + py;
			dx = -Blocksize;
			dy = dx * nTan;
		}
		else if (newAngle == PI / 2 || newAngle == (3 * PI) / 2) {
			rhx = px;
			rhy = py;
			dof = 15;
		}

		while (dof < 15) { //PREVERI ALI SI SE ZALETU V 15 KORAKIH
			wx = rhx / Blocksize;
			wy = rhy / Blocksize;
			if (wx > -1 && wx < sizeof(map)/sizeof(map[0]) && wy > -1 && wy < sizeof(map)/sizeof(map[0]) && map[wy][wx] != 0) {
				dof = 15;
				hmt = map[wy][wx];
			}
			else {
				rhx += dx;
				rhy += dy;
				dof++;
			}
		}
		d2 = sqrt((px - rhx) * (px - rhx) + (py - rhy) * (py - rhy));

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
		}
		else {
			//glColor3f(255 / 255, 109 / 255, 119 / 255);
			//line(px, py, rvx, rvy);
			d = d1;
			rx = rvx;
			ry = rvy;
			hmt = vmt; //DA SE OBARVAJO VSE STRANI
		}
		//NARIŠI STENE
		float lineH = (Blocksize * 100) / (d * cosf(z * DEGREE)); //POPRAVI FISHEYE
		float ty_step = 32.0f / (float)lineH; //ZATO DA SE TEKSTURA ENAKOMERNO PORAZDELI
		float ty_offset = 0; //ZATO DA SE TEKSTURE LEPO PORAZDELIJO
		if (lineH > height) {
			ty_offset = (lineH - height) / 2.0f;
			lineH = height;
		}
		float lineOff = 50 - lineH / 2;
		float ty = ty_offset * ty_step; //+ (hmt - 1) * 32;
		float scale = (float)Blocksize / 32.0f;
		float tx = 0;
		if (shade == 1) {
			tx = (int)(rx / scale) % 32;
			if (newAngle < 2 * PI) tx = 31 - tx;
		}
		else {
			tx = (int)(ry / scale) % 32;
			if (newAngle > PI / 2 && newAngle < (3 * PI) / 2) tx = 31 - tx;
		}

		//glPointSize(25);
		//glBegin(GL_POINTS);
		for (int j = 0;j < lineH;j++) {

			int pixel = ((int)ty * 32 + (int)tx) * 3;
			//cout << pixel << endl;
			int red = 0, blue = 0, green = 0;
			switch ((int)hmt)
			{
			case 1:
				red = t_brickWall[pixel + 0] * shade;
				green = t_brickWall[pixel + 1] * shade;
				blue = t_brickWall[pixel + 2] * shade;
				break;
			case 2:
				red = t_wall[pixel + 0] * shade;
				green = t_wall[pixel + 1] * shade;
				blue = t_wall[pixel + 2] * shade;
				break;
			case 3:
				red = t_wood[pixel + 0] * shade;
				green = t_wood[pixel + 1] * shade;
				blue = t_wood[pixel + 2] * shade;
				break;

			default:
				break;
			}
			int wallx = (int)((float)(z+30)*width/60);
			int wally = lineOff + j;

			wallx *= s_scale;
			wallx += s_offset;

			wally *= s_scale;

			narisi_velik_kvadrat_stene(wallx, wally, create_rgb(red, green, blue));
			ty += ty_step;
		}
		//glEnd();

		//NARIŠI TLA IN STROP
		//glPointSize(25);
		//glBegin(GL_POINTS);
		for (int y = lineOff + lineH;y < height;y++) {
			float dy = y - (height / 2.0f);
			float floorAngle = (angle - newAngle);
			if (floorAngle < 0) floorAngle += 2 * PI;
			else if (floorAngle > 2 * PI) floorAngle -= 2 * PI;
			//floorAngle = cosf(floorAngle);
			//tx = px / 0.3 + cosf(newAngle) * 49 * 32 / dy / cosf(floorAngle);
			//ty = py / 0.3 + sinf(newAngle) * 49 * 32 / dy / cosf(floorAngle);
			tx = px / 0.3 + cosf(newAngle) * 57.735 * 32 / dy / cosf(floorAngle);
			ty = py / 0.3 + sinf(newAngle) * 57.735 * 32 / dy / cosf(floorAngle);
			int pixel = (((int)(ty) & 31) * 32 + ((int)(tx)&31)) * 3;
			int red = t_floor[pixel + 0] * 0.7;
			int green = t_floor[pixel + 1] * 0.7;
			int blue = t_floor[pixel + 2] * 0.7;
			//glColor3ub(red, green, blue);
			//glVertex2i((float)(z + 30)* width / 60, y);
			int tlax = (int)((float)(z+30)*width/60);
			int tlay = y;

			tlax *= s_scale;
			tlax += s_offset;

			tlay *= s_scale;

			narisi_velik_kvadrat_stene(tlax, tlay, create_rgb(red, green, blue));

			//UG_FillFrame(tlax, tlay, tlax, tlay, create_rgb(red, green, blue));

			//NARISI STROP
			pixel = (((int)(ty) & 31) * 32 + ((int)(tx) & 31)) * 3;
			red = t_wall[pixel + 0] * 0.7;
			green = t_wall[pixel + 1] * 0.7;
			blue = t_wall[pixel + 2] * 0.7;
			//glColor3ub(red, green, blue);
			//glVertex2i((float)(z + 30)* width / 60,height -  y);
			int stropx = (int)((float)((z+30)*width)/60);
			int stropy = height - y;

			stropx *= s_scale;
			stropx += s_offset;

			stropy *= s_scale;

			narisi_velik_kvadrat_stene(stropx, stropy, create_rgb(red, green, blue));
		}




	}
}
