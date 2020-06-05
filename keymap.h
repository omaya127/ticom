#ifndef _KEYMAP_H_
#define _KEYMAP_H_

#define KEY_CTRL_A  0x01
#define KEY_CTRL_Q  0x11

#define SUPER_KEY   KEY_CTRL_A

int keymap(const char *buf, int len);

#endif
