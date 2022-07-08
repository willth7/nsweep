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

#ifndef _NSWEEP_ASCII_H
#define _NSWEEP_ASCII_H

#include <stdint.h>

uint32_t ascii_pos[8192];
uint32_t ascii_tex[8192];
uint32_t ascii_ind[5120];

struct text_s {
	uint8_t* c;
	uint16_t x;
	uint16_t y;
	uint8_t sz;
};

uint32_t rndr_text(struct text_s, uint32_t);

#endif