#ifndef _TERM_H_
#define _TERM_H_

#include <termios.h>

int tty_save(int fd);
int tty_reset(int fd);
int tty_raw(int fd);
int tty_set(int fd, char *baudrate, char *databit, char *parity, char *stopbit);
int tty_std(int fd);

#endif
