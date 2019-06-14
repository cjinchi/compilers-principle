#ifndef MIPS_TRANSLATE_H
#define MIPS_TRANSLATE_H

#include "InterCode.h"

void print_mips_code(InterCode* codes);

extern FILE* dst;

void print_header_code();

#endif