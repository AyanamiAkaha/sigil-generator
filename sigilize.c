/*
 * Copyright (c) 2019 Ayanami Akaha
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <errno.h>
#include <string.h>

#include "sigilize_config.h"
#include "sigilize.h"
#include "process_intent.h"
#include "draw.h"

static struct option longopts[] = {
	{ "help", no_argument, NULL, 'h' },
	{ "version", no_argument, NULL, 'v' },
	{ "output", required_argument, NULL, 'o' },
	{ NULL, 0, NULL, 0 }
};

typedef struct {
	char const * fn_out;
	char * intent;
	void * sigil_data;
} params_t;

static char const * progname;

void version() {
	fprintf(
			stderr,
			"260's Sigilizer v%d.%d.%d, (c) 2019 Frater 260 <260@ciemnosc.com>\n",
			Sigilize_VERSION_MAJOR,
			Sigilize_VERSION_MINOR,
			Sigilize_VERSION_PATCH
			);
}

void usage() {
	version();
	fprintf(stderr, "Usage:\n\t%s [options] intent\n\n", progname);
	fprintf(stderr, "Options:\n");
	fprintf(stderr, "\t-h, --help\tprint this help and exit\n");
	fprintf(stderr, "\t-v, --version\tprint program version and exit\n");
	fprintf(stderr, "\t-o, --output <file>\twrite to file instead of stdout. Defaults to sigil.png\n");
}

params_t parse_args(int argc, char * const * argv) {
	params_t params;
	params.fn_out = "sigil.png";
	char ch;
	while((ch = getopt_long(argc, argv, "hvo:", longopts, NULL)) != -1) {
		switch(ch) {
			case 'h':
				usage();
				exit(0);
			case 'v':
				version();
				exit(0);
			case 'o':
				params.fn_out = argv[optind];
				break;
			default:
				error_exit(EINVAL, "Unknown option %c\n\n", ch);
		}
	}
	if(optind >= argc) {
		error_exit(EINVAL, "Missing intent\n\n");
	}
	int len = strlen(argv[optind]);
	params.intent = malloc(len+1);
	if(params.intent == NULL) {
		error_exit(ENOMEM, "unable to allocate memory for intent");
	}
	params.intent = strcpy(params.intent, argv[optind]);
	return params;
}

params_t encode_intent(params_t params) {
	sigil_t sigil = process_intent(params.intent);
	params.sigil_data = (void*)(&sigil);
	return params;
}

void write_sigil(params_t params) {
	if(params.sigil_data == NULL) {
		error_exit(EINVAL, "Sigil data is null\n\n");
	}
	sigil_t* sigil = (sigil_t*)params.sigil_data;
	cairo_t* cr = create_cr();
	int i;
	for(i=0; i<sigil->nshapes; i++) {
		draw_shape(cr, sigil->shapes[i]);
	}
	cairo_surface_write_to_png(cairo_get_target(cr), params.fn_out);
}

int main(int argc, char **argv) {
	progname = argv[0];
	params_t params = parse_args(argc, argv);
	params = encode_intent(params);
	write_sigil(params);
}
