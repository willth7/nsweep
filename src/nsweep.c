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

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "gfx/gfx.h"
#include "img/img.h"
#include "img/bmp.h"

#include "nsweep/field.h"
#include "nsweep/ascii.h"

gfx_t* gfx;
gfx_win_t* win;
uint8_t scene;

struct text_s w_box;
struct text_s h_box;
struct text_s m_box;
struct text_s _box;

struct text_s* a_box;

uint32_t uni[2];

void mouse_cb(GLFWwindow* win, int32_t but, int32_t act, int32_t mod) {
	if (but == GLFW_MOUSE_BUTTON_LEFT && act == GLFW_PRESS && !scene) {
		double dx;
		double dy;
		glfwGetCursorPos(win, &dx, &dy);
		int8_t x = ((int64_t) dx) / tile_sz;
		int8_t y = ((int64_t) dy) / tile_sz;
		lprs_field(x, y - 1);
	}
	else if (but == GLFW_MOUSE_BUTTON_RIGHT && act == GLFW_PRESS && !scene) {
		double dx;
		double dy;
		glfwGetCursorPos(win, &dx, &dy);
		int8_t x = ((int64_t) dx) / tile_sz;
		int8_t y = ((int64_t) dy) / tile_sz;
		rprs_field(x, y - 1);
	}
}

void key_cb(GLFWwindow* glfw_win, int32_t key, int32_t scan, int32_t act, int32_t mod) {
	if (key == GLFW_KEY_ESCAPE && act == GLFW_PRESS) glfwSetWindowShouldClose(glfw_win, 1);
	else if (key == GLFW_KEY_TAB && act == GLFW_PRESS && !scene) {
		uni[0] = tile_sz * 5;
		uni[1] = tile_sz * 5;
		gfx_resz(gfx, win, tile_sz * 5, tile_sz * 5);
		glfwSetWindowSize(glfw_win, tile_sz * 5, tile_sz * 5);
		scene = 1;
	}
	else if (key == GLFW_KEY_BACKSPACE && act == GLFW_PRESS && scene) a_box->c[strlen(a_box->c) - 1] = '\0';
	else if (key == GLFW_KEY_UP && act == GLFW_PRESS && scene) {
		if (a_box == &w_box) a_box = &m_box;
		else if (a_box == &h_box) a_box = &w_box;
		else if (a_box == &m_box) a_box = &h_box;
	}
	else if (key == GLFW_KEY_DOWN && act == GLFW_PRESS && scene) {
		if (a_box == &w_box) a_box = &h_box;
		else if (a_box == &h_box) a_box = &m_box;
		else if (a_box == &m_box) a_box = &w_box;
	}
	else if (key == GLFW_KEY_ENTER && act == GLFW_PRESS && scene) {
		w = atoi(w_box.c);
		h = atoi(h_box.c);
		m = atoi(m_box.c);
		if (w > 0 && w <= 32 && h > 0 && h <= 32 && m > 0 && m <= 1023) {
			uni[0] = w * tile_sz;
			uni[1] = (h + 1) * tile_sz;
			gfx_resz(gfx, win, w * tile_sz, (h + 1) * tile_sz);
			glfwSetWindowSize(glfw_win, w * tile_sz, (h + 1) * tile_sz);
			
			free(field);
			free(flag);
			init_field();
			scene = 0;
		}
	}
}

void char_cb (GLFWwindow* glfw_win, uint32_t c) {
	if (scene) {
		if (!strcmp(a_box->c, "width") || !strcmp(a_box->c, "height") || !strcmp(a_box->c, "mines")) a_box->c[0] = '\0';
		uint8_t n = strlen(a_box->c);
		a_box->c[n] = c;
		a_box->c[n + 1] = '\0';
	}
}

int16_t main() {
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	GLFWwindow* glfw_win = glfwCreateWindow(1, 1, "nsweep", 0, 0);
	glfwSetWindowAttrib(glfw_win, GLFW_RESIZABLE, GLFW_FALSE);
	glfwSetKeyCallback(glfw_win, key_cb);
	glfwSetMouseButtonCallback(glfw_win, mouse_cb);
	glfwSetCharCallback(glfw_win, char_cb);
	
	const GLFWvidmode* vid = glfwGetVideoMode(glfwGetPrimaryMonitor());
	if (vid->width > 1000) {
		tile_sz = 20;
		text_sz = 7;

	}
	if (vid->width > 2000) {
		tile_sz = 30;
		text_sz = 12;

	}
	if (vid->width > 3000) {
		tile_sz = 40;
		text_sz = 15;

	}
	
	img_t* img = bmp_read("img/mine.bmp");
	img_flip_h(img);
	GLFWimage* glfw_img = malloc(sizeof(GLFWimage));
	glfw_img->pixels = img->pix;
	glfw_img->width = img->w;
	glfw_img->height = img->h;
	glfwSetWindowIcon(glfw_win, 1, glfw_img);
	free(glfw_img);
	img_clr(img);
	
	gfx = gfx_init(0);
	gfx_cmd_t* cmd = gfx_cmd_init(gfx);
	
	gfx_vrtx_t* field_vrtx = gfx_vrtx_init(gfx, 2, 2, sizeof(field_pos));
	gfx_vrtx_bind(field_vrtx, 0, 8);
	gfx_vrtx_bind(field_vrtx, 1, 8);
	gfx_vrtx_attr(field_vrtx, 0, 0, 8, 0);
	gfx_vrtx_attr(field_vrtx, 1, 1, 8, 0);
	gfx_vrtx_in(field_vrtx);
	
	gfx_bfr_t* field_indx = gfx_indx_init(gfx, sizeof(field_ind));
	
	gfx_vrtx_t* ascii_vrtx = gfx_vrtx_init(gfx, 2, 2, sizeof(ascii_pos));
	gfx_vrtx_bind(ascii_vrtx, 0, 8);
	gfx_vrtx_bind(ascii_vrtx, 1, 8);
	gfx_vrtx_attr(ascii_vrtx, 0, 0, 8, 0);
	gfx_vrtx_attr(ascii_vrtx, 1, 1, 8, 0);
	gfx_vrtx_in(ascii_vrtx);
	
	gfx_bfr_t* ascii_indx = gfx_indx_init(gfx, sizeof(ascii_ind));
	
	img = bmp_read("img/atlas.bmp");
	img_flip_h(img);
	gfx_txtr_t* atlas = gfx_txtr_init(gfx, cmd, img->pix, img->w, img->h);
	img_clr(img);
	
	img = bmp_read("img/ascii_white.bmp");
	img_flip_h(img);
	gfx_txtr_t* ascii = gfx_txtr_init(gfx, cmd, img->pix, img->w, img->h);
	img_clr(img);
	
	gfx_dscr_t* dscr = gfx_dscr_init(gfx, 2);
	gfx_dscr_writ(gfx, dscr, 0, 0, 0, 0, atlas);
	gfx_dscr_writ(gfx, dscr, 1, 0, 0, 0, ascii);
	
	win = gfx_win_init(gfx, glfw_win);
	gfx_rndr_init(gfx, win);
	
	gfx_pipe_t* field_pipe = gfx_pipe_init(gfx, win, "shd/field_vrtx.spv", "shd/field_frag.spv", field_vrtx, dscr, sizeof(uni));
	gfx_pipe_t* ascii_pipe = gfx_pipe_init(gfx, win, "shd/ascii_vrtx.spv", "shd/ascii_frag.spv", ascii_vrtx, dscr, sizeof(uni));
	
	gfx_swap_init(gfx, win);
	gfx_dpth_init(gfx, win);
	gfx_frme_init(gfx, win);
	
	gfx_clr(win, 24, 24, 24);
	
	uni[0] = w * tile_sz;
	uni[1] = (h + 1) * tile_sz;
	gfx_resz(gfx, win, w * tile_sz, (h + 1) * tile_sz);
	glfwSetWindowSize(glfw_win, w * tile_sz, (h + 1) * tile_sz);
	
	init_field();
	
	w_box.c = calloc(8, 1);
	w_box.x = text_sz;
	w_box.y = text_sz;
	w_box.sz = text_sz;
	sprintf(w_box.c, "%s", "width");
	
	h_box.c = calloc(8, 1);
	h_box.x = text_sz;
	h_box.y = text_sz * 5;
	h_box.sz = text_sz;
	sprintf(h_box.c, "%s", "height");
	
	m_box.c = calloc(8, 1);
	m_box.x = text_sz;
	m_box.y = text_sz * 9;
	m_box.sz = text_sz;
	sprintf(m_box.c, "%s", "mines");
	
	_box.c = calloc(8, 1);
	_box.x = 6900;
	_box.y = 6900;
	_box.sz = 1;
	sprintf(_box.c, "%s", "____");
	
	a_box = &w_box;
	
	while(!glfwWindowShouldClose(glfw_win)) {
		glfwPollEvents();
		
		gfx_vrtx_rfsh(gfx, field_vrtx, 0, field_pos, sizeof(field_pos));
		gfx_vrtx_rfsh(gfx, field_vrtx, 1, field_tex, sizeof(field_tex));
		gfx_bfr_rfsh(gfx, field_indx, field_ind, sizeof(field_ind));
		
		gfx_vrtx_rfsh(gfx, ascii_vrtx, 0, ascii_pos, sizeof(ascii_pos));
		gfx_vrtx_rfsh(gfx, ascii_vrtx, 1, ascii_tex, sizeof(ascii_tex));
		gfx_bfr_rfsh(gfx, ascii_indx, ascii_ind, sizeof(ascii_ind));
		
		gfx_next(gfx, win, cmd);
		if (!scene) {
			uint32_t i = 0;
			i = rndr_text(scr, 0);
			i = rndr_text(_box, i);
			gfx_draw(gfx, win, field_pipe, cmd, field_indx, field_vrtx, dscr, uni, sizeof(uni), w * h * 5, 0, 0);
			gfx_draw(gfx, win, ascii_pipe, cmd, ascii_indx, ascii_vrtx, dscr, uni, sizeof(uni), i * 5, 0, 0);
		}
		else if (scene) {
			uint32_t i = 0;
			i = rndr_text(w_box, 0);
			i = rndr_text(h_box, i);
			i = rndr_text(m_box, i);
			i = rndr_text(_box, i);
			gfx_draw(gfx, win, ascii_pipe, cmd, ascii_indx, ascii_vrtx, dscr, uni, sizeof(uni), i * 5, 0, 0);
		}
		gfx_swap(gfx, win, cmd);
	}
	free(w_box.c);
	free(h_box.c);
	free(m_box.c);
	free(_box.c);
	
	free(scr.c);
	if (field != 0) free(field);
	if (field != 0) free(flag);
	
	gfx_vrtx_free(gfx, field_vrtx);
	gfx_bfr_free(gfx, field_indx);
	gfx_vrtx_free(gfx, ascii_vrtx);
	gfx_bfr_free(gfx, ascii_indx);
	
	gfx_txtr_free(gfx, atlas);
	gfx_txtr_free(gfx, ascii);
	gfx_dscr_free(gfx, dscr);
	
	gfx_pipe_free(gfx, field_pipe);
	gfx_pipe_free(gfx, ascii_pipe);
	
	gfx_win_free(gfx, win);
	gfx_cmd_free(gfx, cmd);
	gfx_free(gfx);
	
	glfwTerminate();
	return 0;
}
