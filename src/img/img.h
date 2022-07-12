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

#ifndef _IMG_IMG_H
#define _IMG_IMG_H

#include <stdint.h>

#define IMG_R8G8B8 0
#define IMG_R8G8B8A8 1

typedef struct img_s {
	uint8_t* pix;
	uint8_t type;
	uint32_t w;
	uint32_t h;
} img_t;

img_t* img_init(uint8_t, uint32_t, uint32_t);

img_t* img_init_raw(uint8_t, uint32_t, uint32_t, uint8_t*, uint8_t, uint8_t);

void img_resz(img_t*, uint32_t, uint32_t);

void img_flip_h(img_t*);

void img_flip_v(img_t*);

void img_rot_cw(img_t*);

void img_rot_ccw(img_t*);

void img_clr(img_t*);

#endif
