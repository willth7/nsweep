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

#ifndef _NSWEEP_FIELD_H
#define _NSWEEP_FIELD_H

#include <stdint.h>

uint8_t* field;
uint8_t* flag;
uint8_t w;
uint8_t h;
int16_t f;
uint16_t m;
int8_t run;

uint8_t tile_w ;

uint32_t field_pos[8192];
uint32_t field_tex[8192];
uint32_t field_ind[5120];

struct text_s scr;

void init_field();

void set_field(uint8_t, uint8_t, uint8_t);

void deft();

void lprs_field(uint8_t, uint8_t);

void rprs_field(uint8_t, uint8_t);

#endif
