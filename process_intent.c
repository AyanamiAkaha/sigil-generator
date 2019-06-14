#include <stdlib.h>
#include <string.h>
#include "process_intent.h"

#define SATURATION 0.7
#define VALUE 0.7

typedef enum {
	color,
	pointcount,
	points
} state_t;

typedef struct {
	unsigned char nbits;
	unsigned short bits;
} bitbuffer_t;

typedef struct {
	state_t state;
	unsigned char npoints;
	unsigned char current_point;
	bitbuffer_t bitbuffer;
} context_t;

hsv_t get_color(bitbuffer_t* bitbuffer) {
	hsv_t hsv;
	hsv.s = SATURATION;
	hsv.v = VALUE;
	hsv.h = ((float)(bitbuffer->bits & 0xff)) / 256 * 360;
	bitbuffer->bits >>= 8;
	bitbuffer->nbits -=8;
	return hsv;
}

unsigned char numpoints(bitbuffer_t* bitbuffer) {
	unsigned char n = bitbuffer->bits & 0xf;
	bitbuffer->nbits -= 4;
	bitbuffer->bits >>= 4;
	return n;
}

void parse_step(context_t* ctx, shapedef_t* shape) {
	switch(ctx->state) {
		case color:
			shape->color = get_color(&ctx->bitbuffer);
			ctx->state = pointcount;
			break;
		case pointcount:
			shape->numpoints = numpoints(&ctx->bitbuffer);
			shape->points = malloc(shape->numpoints * sizeof(point_t));
			ctx->npoints = shape->numpoints;
			ctx->current_point = 0;
			ctx->state = points;
			break;
		case points:
			// TODO
			break;
	}
}

sigil_t process_intent(char const * intent) {
	int len = strlen(intent);
	bitbuffer_t bitbuffer;
	int idx;

	bitbuffer.bits = *intent;
	bitbuffer.nbits = 8;

	context_t ctx;
	sigil_t sigil;
	// just allocate for worst-case scenario of having all single points
	sigil.shapes = malloc((len/4+1) * sizeof(shapedef_t));
	sigil.nshapes = 0;
	for(idx=1;; idx++) {
		int i = idx % len;
		if(bitbuffer.bits <= 8) {
			bitbuffer.bits |= (intent[i] << bitbuffer.bits);
			bitbuffer.bits += 8;
		}
		parse_step(&ctx, sigil.shapes+sigil.nshapes);
		sigil.nshapes++;

		if(ctx.state == points && ctx.npoints == ctx.npoints) {
			ctx.state = color;
			// if we wrapped around we have all shapes we want
			if(idx > i) break;
		}
	}
	return sigil;
}
