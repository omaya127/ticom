#ifndef _PARSE_H_
#define _PARSE_H_

#define KEY_CTRL_A  0x01
#define KEY_CTRL_Q  0x11

#define SUPER_KEY   KEY_CTRL_A

int parse_keys(const char *buf, int len);

#endif
