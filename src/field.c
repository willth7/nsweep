//   Copyright 2022 Will Thomas
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "nsweep/ascii.h"

uint8_t* field;
uint8_t* flag;
uint8_t w = 20;
uint8_t h = 24;
int16_t f;
uint16_t m = 100;
int8_t run;

uint8_t tile_sz = 10;
uint8_t text_sz = 4;


uint32_t field_pos[8192];
uint32_t field_tex[8192];
uint32_t field_ind[5120];

struct text_s scr;

void init_field() {
	field = calloc(w * h, 1);
	flag = calloc(w * h, 1);
	f = m;
	
	scr.c = calloc(8, 1);
	scr.x = 0;
	scr.y = 2;
	scr.sz = text_sz;
	
	uint16_t i = 0;
	for (uint8_t hi = 0; hi < h; hi++) {
		for (uint8_t wi = 0; wi < w; wi++) {
			field_pos[i * 8 + 0] = wi * tile_sz;
			field_pos[i * 8 + 1] = (hi + 1) * tile_sz;
			
			field_pos[i * 8 + 2] = wi * tile_sz;
			field_pos[i * 8 + 3] = (hi + 2) * tile_sz;
			
			field_pos[i * 8 + 4] = (wi + 1) * tile_sz;
			field_pos[i * 8 + 5] = (hi + 1) * tile_sz;
			
			field_pos[i * 8 + 6] = (wi + 1) * tile_sz;
			field_pos[i * 8 + 7] = (hi + 2) * tile_sz;
			
			field_tex[i * 8 + 0] = 11;
			field_tex[i * 8 + 1] = 0;
			
			field_tex[i * 8 + 2] = 11;
			field_tex[i * 8 + 3] = 1;
			
			field_tex[i * 8 + 4] = 12;
			field_tex[i * 8 + 5] = 0;
			
			field_tex[i * 8 + 6] = 12;
			field_tex[i * 8 + 7] = 1;
			
			field_ind[i * 5 + 0] = i * 4;
			field_ind[i * 5 + 1] = i * 4 + 1;
			field_ind[i * 5 + 2] = i * 4 + 2;
			field_ind[i * 5 + 3] = i * 4 + 3;
			field_ind[i * 5 + 4] = 0xffffffff;
			
			i++;
		}
	}
	srand((uint64_t) malloc(1));
	for (uint8_t i = 0; i < m;) {
		uint8_t wi = rand();
		while (wi >= w) wi = rand();
		
		uint8_t hi = rand();
		while (hi >= h) hi = rand();
		
		if (*(field + (hi * w) + wi) != 9) {
			*(field + (hi * w) + wi) = 9;
			if (wi > 0 && *(field + (hi * w) + (wi - 1)) != 9) (*(field + (hi * w) + (wi - 1)))++;
			if (hi > 0 && *(field + ((hi - 1) * w) + wi) != 9) (*(field + ((hi - 1) * w) + wi))++;
			if (wi < (w - 1) && *(field + (hi * w) + (wi + 1)) != 9) (*(field + (hi * w) + (wi + 1)))++;
			if (hi < (h - 1) && *(field + ((hi + 1) * w) + wi) != 9) (*(field + ((hi + 1) * w) + wi))++;
			if (wi > 0 && hi > 0 && *(field + ((hi - 1) * w) + (wi - 1)) != 9) (*(field + ((hi - 1) * w) + (wi - 1)))++;
			if (wi > 0 && hi < (h - 1) && *(field + ((hi + 1) * w) + (wi - 1)) != 9) (*(field + ((hi + 1) * w) + (wi - 1)))++;
			if (wi < (w - 1) && hi > 0 && *(field + ((hi - 1) * w) + (wi + 1)) != 9) (*(field + ((hi - 1) * w) + (wi + 1)))++;
			if (wi < (w - 1) && hi < (h - 1) && *(field + ((hi + 1) * w) + (wi + 1)) != 9) (*(field + ((hi + 1) * w) + (wi + 1)))++;
			i++;
		}
	}
	run = 1;
}

void set_field(uint8_t x, uint8_t y, uint8_t a) {
	field_pos[((y * w) + x) * 8 + 0] = x * tile_sz;
	field_pos[((y * w) + x) * 8 + 1] = (y + 1) * tile_sz;
	
	field_pos[((y * w) + x) * 8 + 2] = x * tile_sz;
	field_pos[((y * w) + x) * 8 + 3] = (y + 2) * tile_sz;
	
	field_pos[((y * w) + x) * 8 + 4] = (x + 1) * tile_sz;
	field_pos[((y * w) + x) * 8 + 5] = (y + 1) * tile_sz;
	
	field_pos[((y * w) + x) * 8 + 6] = (x + 1) * tile_sz;
	field_pos[((y * w) + x) * 8 + 7] = (y + 2) * tile_sz;
	
	field_tex[((y * w) + x) * 8 + 0] = a;
	field_tex[((y * w) + x) * 8 + 1] = 0;
	
	field_tex[((y * w) + x) * 8 + 2] = a;
	field_tex[((y * w) + x) * 8 + 3] = 1;
	
	field_tex[((y * w) + x) * 8 + 4] = a + 1;
	field_tex[((y * w) + x) * 8 + 5] = 0;
	
	field_tex[((y * w) + x) * 8 + 6] = a + 1;
	field_tex[((y * w) + x) * 8 + 7] = 1;
}

void vict() {
	for (uint8_t hi = 0; hi < h; hi++) {
		for (uint8_t wi = 0; wi < w; wi++) {
			if (*(flag + (hi * w) + wi) == 0) set_field(wi, hi,  *(field + (hi * w) + wi) == 0);
		}
	}
	sprintf(scr.c, "%s", "victory");
	scr.x = ((w * tile_sz) / 2) - ((strlen(scr.c) * scr.sz) / 2);
}

void deft() {
	for (uint8_t hi = 0; hi < h; hi++) {
		for (uint8_t wi = 0; wi < w; wi++) {
			if (*(field + (hi * w) + wi) == 9 && *(flag + (hi * w) + wi) == 0) set_field(wi, hi, 12);
			if (*(field + (hi * w) + wi) != 9 && *(flag + (hi * w) + wi) == 2) set_field(wi, hi, 10);
		}
	}
	sprintf(scr.c, "%s", "defeat");
	scr.x = ((w * tile_sz) / 2) - ((strlen(scr.c) * scr.sz) / 2);
}

void lprs_field(uint8_t x, uint8_t y) {
	if (!run) return;
	if (*(field + (y * w) + x) == 9 && *(flag + (y * w) + x) == 0) {
		deft();
		run = 0;
		return;
	}
	else if (*(flag + (y * w) + x) == 0) {
		set_field(x, y, *(field + (y * w) + x));
		*(flag + (y * w) + x) = 1;
		if (*(field + (y * w) + x) == 0) {
			if (x > 0) lprs_field(x - 1, y);
			if (y > 0) lprs_field(x, y - 1);
			if (x < (w - 1)) lprs_field(x + 1, y);
			if (y < (h - 1)) lprs_field(x, y + 1);
			if (x > 0 && y > 0) lprs_field(x - 1, y - 1);
			if (x > 0 && y < (h - 1)) lprs_field(x - 1, y + 1);
			if (x < (w - 1) && y > 0) lprs_field(x + 1, y - 1);
			if (x < (w - 1) && y < (h - 1)) lprs_field(x + 1, y + 1);
		}
	}
	else if (*(flag + (y * w) + x) == 1 && *(field + (y * w) + x) != 0) {
		uint8_t fcnt = 0;
		if (x > 0 && *(flag + (y * w) + (x - 1)) == 2) fcnt++;
		if (y > 0 && *(flag + ((y - 1) * w) + x) == 2) fcnt++;
		if (x < (w - 1) && *(flag + (y * w) + (x + 1)) == 2) fcnt++;
		if (y < (h - 1) && *(flag + ((y + 1) * w) + x) == 2) fcnt++;
		if (x > 0 && y > 0 && *(flag + ((y - 1) * w) + (x - 1)) == 2) fcnt++;
		if (x > 0 && y < (h - 1) && *(flag + ((y + 1) * w) + (x - 1)) == 2) fcnt++;
		if (x < (w - 1) && y > 0 && *(flag + ((y - 1) * w) + (x + 1)) == 2) fcnt++;
		if (x < (w - 1) && y < (h - 1) && *(flag + ((y + 1) * w) + (x + 1)) == 2) fcnt++;
		if (*(field + (y * w) + x) == fcnt) {
			if (x > 0 && *(flag + (y * w) + (x - 1)) == 0) lprs_field(x - 1, y);
			if (y > 0 && *(flag + ((y - 1) * w) + x) == 0) lprs_field(x, y - 1);
			if (x < (w - 1) && *(flag + (y * w) + (x + 1)) == 0) lprs_field(x + 1, y);
			if (y < (h - 1) && *(flag + ((y + 1) * w) + x) == 0) lprs_field(x, y + 1);
			if (x > 0 && y > 0 && *(flag + ((y - 1) * w) + (x - 1)) == 0) lprs_field(x - 1, y - 1);
			if (x > 0 && y < (h - 1) && *(flag + ((y + 1) * w) + (x - 1)) == 0) lprs_field(x - 1, y + 1);
			if (x < (w - 1) && y > 0 && *(flag + ((y - 1) * w) + (x + 1)) == 0) lprs_field(x + 1, y - 1);
			if (x < (w - 1) && y < (h - 1) && *(flag + ((y + 1) * w) + (x + 1)) == 0) lprs_field(x + 1, y + 1);
		}
	}
	sprintf(scr.c, "%i", f);
	scr.x = ((w * tile_sz) / 2) - ((strlen(scr.c) * scr.sz) / 2);
}

void rprs_field(uint8_t x, uint8_t y) {
	if (!run) return;
	if (*(flag + (y * w) + x) == 0) {
		*(flag + (y * w) + x) = 2;
		set_field(x, y, 9);
		f--;
		if (*(field + (y * w) + x) == 9) m--;
	}
	else if (*(flag + (y * w) + x) == 2) {
		*(flag + (y * w) + x) = 0;
		set_field(x, y, 11);
		f++;
		if (*(field + (y * w) + x) == 9) m++;
	}
	if (f == 0 && m == 0) {
		run = 0;
		vict();
		return;
	}
	sprintf(scr.c, "%i", f);
	scr.x = ((w * tile_sz) / 2) - ((strlen(scr.c) * scr.sz) / 2);
}
