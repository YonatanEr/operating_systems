#ifndef NAME_PARSING_H
#define NAME_PARSING_H

#include <stdbool.h>

void split_stored(unsigned char name_ext[11], unsigned char name[8], unsigned char ext[3]);

void stored2username(unsigned char name[8], unsigned char ext[3], unsigned char user_name[13]);

void username2stored(unsigned char user_name[13], unsigned char name[8], unsigned char ext[3]);

bool isEqual(unsigned char name[8], unsigned char ext[3], unsigned char user_name[13]);

unsigned char ToUpperCase(unsigned char x);

#endif
