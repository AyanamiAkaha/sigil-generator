#ifndef SIGILIZER_DRAW_H
#define SIGILIZER_DRAW_H

#include <cairo/cairo.h>
#include "colors.h"

typedef struct {
	float x;
	float y;
} point_t;

typedef struct {
	hsv_t color;
	unsigned char numpoints;
	point_t const * points;
} shapedef_t;

cairo_t* create_cr();
void draw_shape(cairo_t *cr, shapedef_t shape);

#endif
