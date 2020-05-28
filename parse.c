#include "parse.h"
#include <stdio.h>

static char key = 0;

int parse_keys(const char *buf, int len)
{
    int i;
    for (i = 0; i < len; i++) {
        /* SUPER_KEY down */
        if (key == SUPER_KEY) {
            switch (buf[i]) {
                case SUPER_KEY:
                    key = 0;
                    break;
                case KEY_CTRL_Q:
                    key = KEY_CTRL_Q;
                    break;
                default:
                    key = 0;
                    break;
            }
        }
        else if(buf[i] == SUPER_KEY) {
            key = SUPER_KEY;
            //printf("super key\n");
        }
    }
    return key;
}
