#ifndef SIGILIZER_COLORS_H
#define SIGILIZER_COLORS_H

#include <cairo/cairo.h>
#include <math.h>

typedef struct {
	float r;
	float g;
	float b;
} rgb_t;

typedef struct {
	float h;
	float s;
	float v;
} hsv_t;

rgb_t hsv2rgb(hsv_t);

#endif
