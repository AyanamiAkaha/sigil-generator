#include <stdlib.h>
#include <string.h>
#include "process_intent.h"
#include "draw.h"

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
	unsigned char n = (bitbuffer->bits & 0xf) + 1;
	bitbuffer->nbits -= 4;
	bitbuffer->bits >>= 4;
	return n;
}

point_t get_point(context_t* ctx) {
	point_t p;
	p.x = (ctx->bitbuffer.bits & 0xf) * (float)IMAGE_WIDTH / 16;
	ctx->bitbuffer.bits >>= 4;
	ctx->bitbuffer.nbits -= 4;
	p.y = (ctx->bitbuffer.bits & 0xf) * (float)IMAGE_HEIGHT / 16;
	ctx->bitbuffer.bits >>= 4;
	ctx->bitbuffer.nbits -= 4;
	return p;
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
			shape->points[ctx->current_point] = get_point(ctx);
			ctx->current_point++;
			break;
	}
}

sigil_t process_intent(char const * intent) {
	int len = strlen(intent);
	int idx;

	context_t ctx;
	sigil_t sigil;

	ctx.bitbuffer.bits = *intent;
	ctx.bitbuffer.nbits = 8;

	// just allocate for worst-case scenario of having all single points
	sigil.shapes = malloc((len/4+1) * sizeof(shapedef_t));
	sigil.nshapes = 0;
	for(idx=1;; idx++) {
		int i = idx % len;
		if(ctx.bitbuffer.nbits <= 8) {
			ctx.bitbuffer.bits |= (intent[i] << ctx.bitbuffer.bits);
			ctx.bitbuffer.nbits += 8;
		}
		parse_step(&ctx, sigil.shapes+sigil.nshapes);
		if(ctx.state == points && ctx.current_point == ctx.npoints) {
			sigil.nshapes++;
			ctx.state = color;
			// if we wrapped around we have all shapes we want
			if(idx > i) break;
		}
	}
	return sigil;
}
