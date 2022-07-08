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

#include "nsweep/field.h"
#include "nsweep/ascii.h"

uint32_t uni[2];

void mouse_cb(GLFWwindow* win, int32_t but, int32_t act, int32_t mod) {
	if (but == GLFW_MOUSE_BUTTON_LEFT && act == GLFW_PRESS) {
		double dx;
		double dy;
		glfwGetCursorPos(win, &dx, &dy);
		int8_t x = ((int64_t) dx) / tile_w;
		int8_t y = ((int64_t) dy) / tile_w;
		lprs_field(x, y - 1);
	}
	else if (but == GLFW_MOUSE_BUTTON_RIGHT && act == GLFW_PRESS) {
		double dx;
		double dy;
		glfwGetCursorPos(win, &dx, &dy);
		int8_t x = ((int64_t) dx) / tile_w;
		int8_t y = ((int64_t) dy) / tile_w;
		rprs_field(x, y - 1);
	}
}

void key_cb(GLFWwindow* win, int32_t key, int32_t scan, int32_t act, int32_t mod) {
	if (key == GLFW_KEY_ESCAPE && act == GLFW_PRESS) glfwSetWindowShouldClose(win, 1);
}

int16_t main() {
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	GLFWwindow* glfw_win = glfwCreateWindow(1, 1, "nsweep", 0, 0);
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
	
	gfx_t* gfx = gfx_init();
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
	
	gfx_win_t* win = gfx_win_init(gfx, glfw_win);
	gfx_rndr_init(gfx, win);
	
	gfx_pipe_t* field_pipe = gfx_pipe_init(gfx, win, "shd/field_vrtx.spv", "shd/field_frag.spv", field_vrtx, dscr, sizeof(uni));
	gfx_pipe_t* ascii_pipe = gfx_pipe_init(gfx, win, "shd/ascii_vrtx.spv", "shd/ascii_frag.spv", ascii_vrtx, dscr, sizeof(uni));
	
	gfx_swap_init(gfx, win);
	gfx_dpth_init(gfx, win);
	gfx_frme_init(gfx, win);
	
	gfx_clr(win, 8, 8, 8);
	
	uni[0] = w * tile_w;
	uni[1] = (h + 1) * tile_w;
	gfx_resz(gfx, win, w * tile_w, (h + 1) * tile_w);
	glfwSetWindowSize(glfw_win, w * tile_w, (h + 1) * tile_w);
	
	init_field();
	
	while(!glfwWindowShouldClose(glfw_win)) {
		glfwPollEvents();
		
		gfx_vrtx_rfsh(gfx, field_vrtx, 0, field_pos, sizeof(field_pos));
		gfx_vrtx_rfsh(gfx, field_vrtx, 1, field_tex, sizeof(field_tex));
		gfx_bfr_rfsh(gfx, field_indx, field_ind, sizeof(field_ind));
		
		gfx_vrtx_rfsh(gfx, ascii_vrtx, 0, ascii_pos, sizeof(ascii_pos));
		gfx_vrtx_rfsh(gfx, ascii_vrtx, 1, ascii_tex, sizeof(ascii_tex));
		gfx_bfr_rfsh(gfx, ascii_indx, ascii_ind, sizeof(ascii_ind));
		
		gfx_next(gfx, win, cmd);
		gfx_draw(gfx, win, field_pipe, cmd, field_indx, field_vrtx, dscr, uni, sizeof(uni), w * h * 5, 0, 0);
		gfx_draw(gfx, win, ascii_pipe, cmd, ascii_indx, ascii_vrtx, dscr, uni, sizeof(uni), rndr_text(scr, 0) * 5, 0, 0);
		gfx_swap(gfx, win, cmd);
	}
	
	free(scr.c);
	free(field);
	free(flag);
	
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
