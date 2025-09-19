#include "string.h"
#include <string.h>
#include <stdlib.h>

void cat(char **s, const char *m) {
    *s = realloc(*s, strlen(*s) + strlen(m) + 1);
    strcat(*s, m);
}

void catChar(char **s, char c) {
    const int l = strlen(*s);
    *s = realloc(*s, l + 2);
    (*s)[l] = c;
    (*s)[l+1] = '\0';
}

void catCodepoint(char **s, int c) {
    char *bleh = utf8encode(c);
    cat(s, bleh);
    free(bleh);
}

char *utf8encode(int codepoint) {
    char *s = strdup("");
    char cs[4] = {};
    if (codepoint < 1 << 7) {
        catChar(&s, codepoint);
        goto glue;
    }

    cs[3] = 0b1000 << 4 | grab(codepoint, 0b111111, 0);
    if (codepoint < 1 << 11) {
        cs[0] = 0b110 << 5 | grab(codepoint, 0b11111, 6);
        goto glue;
    }

    cs[2] = 0b1000 << 4 | grab(codepoint, 0b111111, 6);
    if (codepoint < 1 << 16) {
        cs[0] = 0b1110 << 4 | grab(codepoint, 0b1111, 12);
        goto glue;
    }

    cs[1] = 0b10 << 6 | grab(codepoint, 0b111111, 12);
    cs[0] = 0b1111 << 4 | grab(codepoint, 0b111, 18);

glue:
    for (int i = 0; i < 4; i++)
        if (cs[i])
            catChar(&s, cs[i]);
    return s;
}
