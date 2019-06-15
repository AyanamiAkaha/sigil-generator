#ifndef SIGILIZER_PROCESS_INTENT_H
#define SIGILIZER_PROCESS_INTENT_H

#include "draw.h"

typedef struct {
	int nshapes;
	shapedef_t* shapes;
} sigil_t;

sigil_t process_intent(char const *);
void free_sigil(sigil_t*);

#endif
