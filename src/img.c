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

#define IMG_R8G8B8 0
#define IMG_R8G8B8A8 1

typedef struct img_s {
	uint8_t* pix;
	uint8_t type;
	uint32_t w;
	uint32_t h;
} img_t;

img_t* img_init(uint8_t type, uint32_t w, uint32_t h) {
	if (type == IMG_R8G8B8) {
		img_t* img = malloc(sizeof(img_t));
		img->pix = malloc(24 * w * h);
		img->w = w;
		img->h = h;
		img->type = type;
		return img;
	}
	else if (type == IMG_R8G8B8A8) {
		img_t* img = malloc(sizeof(img_t));
		img->pix = malloc(32 * w * h);
		img->w = w;
		img->h = h;
		img->type = type;
		return img;
	}
	return 0;
}

img_t* img_init_raw(uint8_t type, uint32_t w, uint32_t h, uint8_t* data, uint8_t off, uint8_t p) {
	if (type == IMG_R8G8B8) {
		img_t* img = malloc(sizeof(img_t));
		img->pix = malloc(24 * w * h);
		uint32_t indx = off;
		for (uint32_t hi = 0; hi < h; hi++) {
			for (uint32_t wi = 0; wi < w; wi++) {
				*(img->pix + (hi * w * 3) + (wi * 3)) = *(data + indx + 2);
				*(img->pix + (hi * w * 3) + (wi * 3) + 1) = *(data + indx + 1);
				*(img->pix + (hi * w * 3) + (wi * 3) + 2) = *(data + indx);
				indx += 3;
			}
			indx += p;
		}
		img->type = type;
		img->w = w;
		img->h = h;
		return img;
	}
	else if (type == IMG_R8G8B8A8) {
		img_t* img = malloc(sizeof(img_t));
		img->pix = malloc(32 * w * h);
		uint32_t indx = off;
		for (uint32_t hi = 0; hi < h; hi++) {
			for (uint32_t wi = 0; wi < w; wi++) {
				*(img->pix + (hi * w * 4) + (wi * 4)) = *(data + indx + 2);
				*(img->pix + (hi * w * 4) + (wi * 4) + 1) = *(data + indx + 1);
				*(img->pix + (hi * w * 4) + (wi * 4) + 2) = *(data + indx);
				*(img->pix + (hi * w * 4) + (wi * 4) + 3) = *(data + indx + 3);
				indx += 4;
			}
		}
		img->type = type;
		img->w = w;
		img->h = h;
		return img;
	}
	return 0;
}

void img_resz(img_t* img, uint32_t w, uint32_t h) {
	if (img->type == IMG_R8G8B8) {
		uint8_t* pix = malloc(24 * w * h);
		for (uint32_t hi = 0; hi < h; hi++) {
			for (uint32_t wi = 0; wi < w; wi++) {
				uint64_t r = 0;
				uint64_t g = 0;
				uint64_t b = 0;
				uint64_t cnt = 0;
				for (uint32_t hj = (img->h * hi) / h; hj <= (img->h * (hi + 1)) / h; hj++) {
					for (uint32_t wj = (img->w * wi) / w; wj <= (img->w * (wi + 1)) / w; wj++) {
						if (wj < img->w && hj < img->h) {
							r += *(img->pix + (hj * img->w * 3) + (wj * 3));
							g += *(img->pix + (hj * img->w * 3) + (wj * 3) + 1);
							b += *(img->pix + (hj * img->w * 3) + (wj * 3) + 2);
							cnt++;
						}
					}
				}
				*(pix + (hi * w * 3) + (wi * 3)) = (uint8_t) (r / cnt);
				*(pix + (hi * w * 3) + (wi * 3) + 1) = (uint8_t) (g / cnt);
				*(pix + (hi * w * 3) + (wi * 3) + 2) = (uint8_t) (b / cnt);
			}
		}
		free(img->pix);
		img->pix = pix;
		img->w = w;
		img->h = h;
	}
	else if (img->type == IMG_R8G8B8A8) {
		uint8_t* pix = malloc(32 * w * h);
		for (uint32_t hi = 0; hi < h; hi++) {
			for (uint32_t wi = 0; wi < w; wi++) {
				uint64_t r = 0;
				uint64_t g = 0;
				uint64_t b = 0;
				uint64_t a = 0;
				uint64_t cnt = 0;
				for (uint32_t hj = (img->h * hi) / h; hj <= (img->h * (hi + 1)) / h; hj++) {
					for (uint32_t wj = (img->w * wi) / w; wj <= (img->w * (wi + 1)) / w; wj++) {
						if (wj < img->w && hj < img->h) {
							r += *(img->pix + (hj * img->w * 4) + (wj * 4));
							g += *(img->pix + (hj * img->w * 4) + (wj * 4) + 1);
							b += *(img->pix + (hj * img->w * 4) + (wj * 4) + 2);
							a += *(img->pix + (hj * img->w * 4) + (wj * 4) + 3);
							cnt++;
						}
					}
				}
				*(pix + (hi * w * 4) + (wi * 4)) = (uint8_t) (r / cnt);
				*(pix + (hi * w * 4) + (wi * 4) + 1) = (uint8_t) (g / cnt);
				*(pix + (hi * w * 4) + (wi * 4) + 2) = (uint8_t) (b / cnt);
				*(pix + (hi * w * 4) + (wi * 4) + 3) = (uint8_t) (a / cnt);
			}
		}
		free(img->pix);
		img->pix = pix;
		img->w = w;
		img->h = h;
	}
}

void img_flip_h(img_t* img) {
	if (img->type == IMG_R8G8B8) {
		uint8_t* pix = malloc(24 * img->w * img->h);
		for (uint32_t hi = 0; hi < img->h; hi++) {
			for (uint32_t wi = 0; wi < img->w; wi++) {
				*(pix + (hi * img->w * 3) + (wi * 3)) = *(img->pix + ((img->h - (hi + 1)) * img->w * 3) + (wi * 3));
				*(pix + (hi * img->w * 3) + (wi * 3) + 1) = *(img->pix + ((img->h - (hi + 1)) * img->w * 3) + (wi * 3) + 1);
				*(pix + (hi * img->w * 3) + (wi * 3) + 2) = *(img->pix + ((img->h - (hi + 1)) * img->w * 3) + (wi * 3) + 2);
			}
		}
		free(img->pix);
		img->pix = pix;
	}
	else if (img->type == IMG_R8G8B8A8) {
		uint8_t* pix = malloc(32 * img->w * img->h);
		for (uint32_t hi = 0; hi < img->h; hi++) {
			for (uint32_t wi = 0; wi < img->w; wi++) {
				*(pix + (hi * img->w * 4) + (wi * 4)) = *(img->pix + ((img->h - (hi + 1)) * img->w * 4) + (wi * 4));
				*(pix + (hi * img->w * 4) + (wi * 4) + 1) = *(img->pix + ((img->h - (hi + 1)) * img->w * 4) + (wi * 4) + 1);
				*(pix + (hi * img->w * 4) + (wi * 4) + 2) = *(img->pix + ((img->h - (hi + 1)) * img->w * 4) + (wi * 4) + 2);
				*(pix + (hi * img->w * 4) + (wi * 4) + 3) = *(img->pix + ((img->h - (hi + 1)) * img->w * 4) + (wi * 4) + 3);
			}
		}
		free(img->pix);
		img->pix = pix;
	}
}

void img_flip_v(img_t* img) {
	if (img->type == IMG_R8G8B8) {
		uint8_t* pix = malloc(24 * img->w * img->h);
		for (uint32_t hi = 0; hi < img->h; hi++) {
			for (uint32_t wi = 0; wi < img->w; wi++) {
				*(pix + (hi * img->w * 3) + (wi * 3)) = *(img->pix + (hi * img->w * 3) + ((img->w - (wi + 1)) * 3));
				*(pix + (hi * img->w * 3) + (wi * 3) + 1) = *(img->pix + (hi * img->w * 3) + ((img->w - (wi + 1)) * 3) + 1);
				*(pix + (hi * img->w * 3) + (wi * 3) + 2) = *(img->pix + (hi * img->w * 3) + ((img->w - (wi + 1)) * 3) + 2);
			}
		}
		free(img->pix);
		img->pix = pix;
	}
	else if (img->type == IMG_R8G8B8A8) {
		uint8_t* pix = malloc(32 * img->w * img->h);
		for (uint32_t hi = 0; hi < img->h; hi++) {
			for (uint32_t wi = 0; wi < img->w; wi++) {
				*(pix + (hi * img->w * 4) + (wi * 4)) = *(img->pix + (hi * img->w * 4) + ((img->w - (wi + 1)) * 4));
				*(pix + (hi * img->w * 4) + (wi * 4) + 1) = *(img->pix + (hi * img->w * 4) + ((img->w - (wi + 1)) * 4) + 1);
				*(pix + (hi * img->w * 4) + (wi * 4) + 2) = *(img->pix + (hi * img->w * 4) + ((img->w - (wi + 1)) * 4) + 2);
				*(pix + (hi * img->w * 4) + (wi * 4) + 3) = *(img->pix + (hi * img->w * 4) + ((img->w - (wi + 1)) * 4) + 3);
			}
		}
		free(img->pix);
		img->pix = pix;
	}
}

void img_rot_cw(img_t* img) {
	if (img->type == IMG_R8G8B8) {
		uint8_t* pix = malloc(24 * img->w * img->h);
		uint32_t w = img->h;
		uint32_t h = img->w;
		for (uint32_t hi = 0; hi < h; hi++) {
			for (uint32_t wi = 0; wi < w; wi++) {
				*(pix + (hi * w * 3) + (wi * 3)) = *(img->pix + (wi * img->w * 3) + ((img->w - (hi + 1)) * 3));
				*(pix + (hi * w * 3) + (wi * 3) + 1) = *(img->pix + (wi * img->w * 3) + ((img->w - (hi + 1)) * 3) + 1);
				*(pix + (hi * w * 3) + (wi * 3) + 2) = *(img->pix + (wi * img->w * 3) + ((img->w - (hi + 1)) * 3) + 2);
			}
		}
		free(img->pix);
		img->pix = pix;
		img->w = w;
		img->h = h;
	}
	else if (img->type == IMG_R8G8B8A8) {
		uint8_t* pix = malloc(32 * img->w * img->h);
		uint32_t w = img->h;
		uint32_t h = img->w;
		for (uint32_t hi = 0; hi < h; hi++) {
			for (uint32_t wi = 0; wi < w; wi++) {
				*(pix + (hi * w * 4) + (wi * 4)) = *(img->pix + (wi * img->w * 4) + ((img->w - (hi + 1)) * 4));
				*(pix + (hi * w * 4) + (wi * 4) + 1) = *(img->pix + (wi * img->w * 4) + ((img->w - (hi + 1)) * 4) + 1);
				*(pix + (hi * w * 4) + (wi * 4) + 2) = *(img->pix + (wi * img->w * 4) + ((img->w - (hi + 1)) * 4) + 2);
				*(pix + (hi * w * 4) + (wi * 4) + 3) = *(img->pix + (wi * img->w * 4) + ((img->w - (hi + 1)) * 4) + 3);
			}
		}
		free(img->pix);
		img->pix = pix;
		img->w = w;
		img->h = h;
	}
}

void img_rot_ccw(img_t* img) {
	if (img->type == IMG_R8G8B8) {
		uint8_t* pix = malloc(24 * img->w * img->h);
		uint32_t w = img->h;
		uint32_t h = img->w;
		for (uint32_t hi = 0; hi < h; hi++) {
			for (uint32_t wi = 0; wi < w; wi++) {
				*(pix + (hi * w * 3) + (wi * 3)) = *(img->pix + ((img->h - (wi + 1)) * img->w * 3) + (hi * 3));
				*(pix + (hi * w * 3) + (wi * 3) + 1) = *(img->pix + ((img->h - (wi + 1)) * img->w * 3) + (hi * 3) + 1);
				*(pix + (hi * w * 3) + (wi * 3) + 2) = *(img->pix + ((img->h - (wi + 1)) * img->w * 3) + (hi * 3) + 2);
			}
		}
		free(img->pix);
		img->pix = pix;
		img->w = w;
		img->h = h;
	}
	else if (img->type == IMG_R8G8B8A8) {
		uint8_t* pix = malloc(32 * img->w * img->h);
		uint32_t w = img->h;
		uint32_t h = img->w;
		for (uint32_t hi = 0; hi < h; hi++) {
			for (uint32_t wi = 0; wi < w; wi++) {
				*(pix + (hi * w * 4) + (wi * 4)) = *(img->pix + ((img->h - (wi + 1)) * img->w * 4) + (hi * 4));
				*(pix + (hi * w * 4) + (wi * 4) + 1) = *(img->pix + ((img->h - (wi + 1)) * img->w * 4) + (hi * 4) + 1);
				*(pix + (hi * w * 4) + (wi * 4) + 2) = *(img->pix + ((img->h - (wi + 1)) * img->w * 4) + (hi * 4) + 2);
				*(pix + (hi * w * 4) + (wi * 4) + 3) = *(img->pix + ((img->h - (wi + 1)) * img->w * 4) + (hi * 4) + 3);
			}
		}
		free(img->pix);
		img->pix = pix;
		img->w = w;
		img->h = h;
	}
}

void img_clr(img_t* img) {
	free(img->pix);
	free(img);
}
