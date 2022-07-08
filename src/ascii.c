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

uint32_t ascii_pos[8192];
uint32_t ascii_tex[8192];
uint32_t ascii_ind[5120];

struct text_s {
	uint8_t* c;
	uint16_t x;
	uint16_t y;
	uint8_t sz;
};

uint32_t rndr_text(struct text_s text, uint32_t off) {
	uint32_t i = off;
	
	for (uint32_t ci = 0; ci < strlen(text.c); ci++) {
		ascii_pos[i * 8 + 0] = text.x;
		ascii_pos[i * 8 + 1] = text.y;
		
		ascii_pos[i * 8 + 2] = text.x;
		ascii_pos[i * 8 + 3] = text.y + text.sz * 2;
		
		ascii_pos[i * 8 + 4] = text.x + text.sz;
		ascii_pos[i * 8 + 5] = text.y;
		
		ascii_pos[i * 8 + 6] = text.x + text.sz;
		ascii_pos[i * 8 + 7] = text.y + text.sz * 2;
		
		ascii_tex[i * 8 + 0] = text.c[ci] - 32;
		ascii_tex[i * 8 + 1] = 0;
		
		ascii_tex[i * 8 + 2] = text.c[ci] - 32;
		ascii_tex[i * 8 + 3] = 1;
		
		ascii_tex[i * 8 + 4] = text.c[ci] - 31;
		ascii_tex[i * 8 + 5] = 0;
		
		ascii_tex[i * 8 + 6] = text.c[ci] - 31;
		ascii_tex[i * 8 + 7] = 1;
		
		ascii_ind[i * 5 + 0] = i * 4;
		ascii_ind[i * 5 + 1] = i * 4 + 1;
		ascii_ind[i * 5 + 2] = i * 4 + 2;
		ascii_ind[i * 5 + 3] = i * 4 + 3;
		ascii_ind[i * 5 + 4] = 0xffffffff;
		
		text.x += text.sz;
		i++;
	}
	
	return i;
}
