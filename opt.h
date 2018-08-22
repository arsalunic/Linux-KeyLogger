#ifndef OPT_H
#define OPT_H

#define UnknownKey  "\0" //unknown key on the keyboard

#include <stdint.h>
#include <stdbool.h>

#include "configuration.h"

void 	parseOpt(int argc, char **argv, Configuration *configuration);
bool 	boolShift(int shiftKey);
char* 	getKey(int shiftKey, int shiftCheck);

#endif