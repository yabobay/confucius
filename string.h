#pragma once

#define grab(x, pattern, pos) (x & 0b111111 << pos) >> pos

void cat(char**, const char*);
void catChar(char**, char);
void catCodepoint(char**, int);
char *utf8encode(int);
