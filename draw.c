#include <cairo/cairo.h>
#include "draw.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define CIRCLE_RADIUS 10
#define LINE_WIDTH 1

#define IMAGE_WIDTH 300
#define IMAGE_HEIGHT 300

cairo_t* create_cr()  {
	cairo_surface_t* surface;
	cairo_t* cr;

	surface = cairo_image_surface_create(CAIRO_FORMAT_RGB24, IMAGE_WIDTH, IMAGE_HEIGHT);
	cr = cairo_create(surface);
	// this only decreases the refcount, which was increased by cairo_create
	cairo_surface_destroy(surface);

	return cr;
}

void init_line(cairo_t *cr, hsv_t hsv) {
	cairo_set_line_width(cr, LINE_WIDTH);
	rgb_t rgb = hsv2rgb(hsv);
	cairo_set_source_rgb(cr, rgb.r, rgb.g, rgb.b);
}

void construct_shape(cairo_t *cr, shapedef_t shape) {
	while(shape.numpoints > 0) {
		if(shape.numpoints < 2) {
			cairo_arc(cr,
					shape.points[0].x,
					shape.points[0].y,
					CIRCLE_RADIUS,
					0, 2*M_PI);
			shape.numpoints--;
		} else if(shape.numpoints == 2) {
			cairo_line_to(cr,
					shape.points[0].x,
					shape.points[0].y);
			cairo_line_to(cr,
					shape.points[1].x,
					shape.points[1].y);
			shape.numpoints--;	// include circle at the end
		} else {
			cairo_curve_to(cr,
					shape.points[0].x,
					shape.points[0].y,
					shape.points[1].x,
					shape.points[1].y,
					shape.points[2].x,
					shape.points[2].y);
			shape.numpoints -= 3;
		}
	}
}

void draw_shape(cairo_t *cr, shapedef_t shape) {
	init_line(cr, shape.color);
	construct_shape(cr, shape);
	cairo_stroke(cr);
}
