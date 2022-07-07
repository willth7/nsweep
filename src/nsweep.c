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

#include <GLFW/glfw3.h>
#include <gfx/gfx.h>
#include <img/img.h>
#include <img/bmp.h>

#include <stdint.h>
#include <stdlib.h>

uint8_t* field;
uint8_t* flag;
uint8_t w = 10;
uint8_t h = 12;
int16_t f;
uint16_t m = 25;
int8_t run;

GLFWwindow* glfw_win;
uint8_t tile_w = 40;

gfx_t* gfx;
gfx_win_t* win;

uint32_t pos[8192];
uint32_t tex[8192];
uint32_t ind[5120];
uint32_t uni[2];

void init_field() {
	gfx_resz(gfx, win, w * tile_w, h * tile_w);
	glfwSetWindowSize(glfw_win, w * tile_w, h * tile_w);
	
	uni[0] = w * tile_w;
	uni[1] = h * tile_w;
	
	field = calloc(w * h, 1);
	flag = calloc(w * h, 1);
	f = m;
	uint16_t i = 0;
	for (uint8_t hi = 0; hi < h; hi++) {
		for (uint8_t wi = 0; wi < w; wi++) {
			pos[i * 8 + 0] = wi * tile_w;
			pos[i * 8 + 1] = hi * tile_w;
			
			pos[i * 8 + 2] = wi * tile_w;
			pos[i * 8 + 3] = (hi + 1) * tile_w;
			
			pos[i * 8 + 4] = (wi + 1) * tile_w;
			pos[i * 8 + 5] = hi * tile_w;
			
			pos[i * 8 + 6] = (wi + 1) * tile_w;
			pos[i * 8 + 7] = (hi + 1) * tile_w;
			
			tex[i * 8 + 0] = 11;
			tex[i * 8 + 1] = 0;
			
			tex[i * 8 + 2] = 11;
			tex[i * 8 + 3] = 1;
			
			tex[i * 8 + 4] = 12;
			tex[i * 8 + 5] = 0;
			
			tex[i * 8 + 6] = 12;
			tex[i * 8 + 7] = 1;
			
			ind[i * 5] = i * 4;
			ind[i * 5 + 1] = i * 4 + 1;
			ind[i * 5 + 2] = i * 4 + 2;
			ind[i * 5 + 3] = i * 4 + 3;
			ind[i * 5 + 4] = 0xffffffff;
			
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
	pos[((y * w) + x) * 8 + 0] = x * tile_w;
	pos[((y * w) + x) * 8 + 1] = y * tile_w;
	
	pos[((y * w) + x) * 8 + 2] = x * tile_w;
	pos[((y * w) + x) * 8 + 3] = (y + 1) * tile_w;
	
	pos[((y * w) + x) * 8 + 4] = (x + 1) * tile_w;
	pos[((y * w) + x) * 8 + 5] = y * tile_w;
	
	pos[((y * w) + x) * 8 + 6] = (x + 1) * tile_w;
	pos[((y * w) + x) * 8 + 7] = (y + 1) * tile_w;
	
	tex[((y * w) + x) * 8 + 0] = a;
	tex[((y * w) + x) * 8 + 1] = 0;
	
	tex[((y * w) + x) * 8 + 2] = a;
	tex[((y * w) + x) * 8 + 3] = 1;
	
	tex[((y * w) + x) * 8 + 4] = a + 1;
	tex[((y * w) + x) * 8 + 5] = 0;
	
	tex[((y * w) + x) * 8 + 6] = a + 1;
	tex[((y * w) + x) * 8 + 7] = 1;
}

void deft() {
	for (uint8_t hi = 0; hi < h; hi++) {
		for (uint8_t wi = 0; wi < w; wi++) {
			if (*(field + (hi * w) + wi) == 9 && *(flag + (hi * w) + wi) == 0) set_field(wi, hi, 12);
			if (*(field + (hi * w) + wi) != 9 && *(flag + (hi * w) + wi) == 2) set_field(wi, hi, 10);
		}
	}
}

void lprs_field(uint8_t x, uint8_t y) {
	if (!run) return;
	if (*(field + (y * w) + x) == 9 && *(flag + (y * w) + x) == 0) {
		deft();
		run = 0;
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
	}
}

void mouse_cb(GLFWwindow* win, int32_t but, int32_t act, int32_t mod) {
	if (but == GLFW_MOUSE_BUTTON_LEFT && act == GLFW_PRESS) {
		double dx;
		double dy;
		glfwGetCursorPos(win, &dx, &dy);
		int8_t x = ((int64_t) dx) / tile_w;
		int8_t y = ((int64_t) dy) / tile_w;
		lprs_field(x, y);
	}
	else if (but == GLFW_MOUSE_BUTTON_RIGHT && act == GLFW_PRESS) {
		double dx;
		double dy;
		glfwGetCursorPos(win, &dx, &dy);
		int8_t x = ((int64_t) dx) / tile_w;
		int8_t y = ((int64_t) dy) / tile_w;
		rprs_field(x, y);
	}
}

void key_cb(GLFWwindow* win, int32_t key, int32_t scan, int32_t act, int32_t mod) {
	if (key == GLFW_KEY_ESCAPE && act == GLFW_PRESS) glfwSetWindowShouldClose(win, 1);
}

int16_t main() {
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfw_win = glfwCreateWindow(1, 1, "nsweep", 0, 0);
	glfwSetWindowAttrib(glfw_win, GLFW_RESIZABLE, GLFW_FALSE);
	glfwSetKeyCallback(glfw_win, key_cb);
	glfwSetMouseButtonCallback(glfw_win, mouse_cb);
	
	img_t* img = bmp_read("img/mine.bmp");
	img_flip_h(img);
	GLFWimage* glfw_img = malloc(sizeof(GLFWimage));
	glfw_img->pixels = img->pix;
	glfw_img->width = img->w;
	glfw_img->height = img->h;
	glfwSetWindowIcon(glfw_win, 1, glfw_img);
	free(glfw_img);
	img_clr(img);
	
	gfx = gfx_init();
	gfx_cmd_t* cmd = gfx_cmd_init(gfx);
	
	gfx_vrtx_t* vrtx = gfx_vrtx_init(gfx, 2, 2, sizeof(pos));
	gfx_vrtx_bind(vrtx, 0, 8);
	gfx_vrtx_bind(vrtx, 1, 8);
	gfx_vrtx_attr(vrtx, 0, 0, 8, 0);
	gfx_vrtx_attr(vrtx, 1, 1, 8, 0);
	gfx_vrtx_in(vrtx);
	
	gfx_bfr_t* indx = gfx_indx_init(gfx, sizeof(ind));
	
	img = bmp_read("img/atlas.bmp");
	img_flip_h(img);
	gfx_txtr_t* atlas = gfx_txtr_init(gfx, cmd, img->pix, img->w, img->h);
	img_clr(img);
	
	gfx_dscr_t* dscr = gfx_dscr_init(gfx, 1);
	gfx_dscr_writ(gfx, dscr, 0, 0, 0, 0, atlas);
	
	win = gfx_win_init(gfx, glfw_win);
	gfx_rndr_init(gfx, win);
	
	gfx_pipe_t* pipe = gfx_pipe_init(gfx, win, "shd/vrtx.spv", "shd/frag.spv", vrtx, dscr, sizeof(uni));
	
	gfx_swap_init(gfx, win);
	gfx_dpth_init(gfx, win);
	gfx_frme_init(gfx, win);
	
	gfx_clr(win, 16, 16, 16);
	
	init_field();
	
	while(!glfwWindowShouldClose(glfw_win)) {
		glfwPollEvents();
		
		gfx_vrtx_rfsh(gfx, vrtx, 0, pos, sizeof(pos));
		gfx_vrtx_rfsh(gfx, vrtx, 1, tex, sizeof(tex));
		gfx_bfr_rfsh(gfx, indx, ind, sizeof(ind));
		
		gfx_next(gfx, win, cmd);
		gfx_draw(gfx, win, pipe, cmd, indx, vrtx, dscr, uni, sizeof(uni), w * h * 5, 0, 0);
		gfx_swap(gfx, win, cmd);
	}
	free(field);
	free(flag);
	
	gfx_vrtx_free(gfx, vrtx);
	gfx_bfr_free(gfx, indx);
	gfx_txtr_free(gfx, atlas);
	gfx_dscr_free(gfx, dscr);
	gfx_pipe_free(gfx, pipe);
	gfx_win_free(gfx, win);
	gfx_cmd_free(gfx, cmd);
	gfx_free(gfx);
	
	glfwTerminate();
	return 0;
}
