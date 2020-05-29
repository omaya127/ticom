#ifndef _TERM_H_
#define _TERM_H_

#include <termios.h>

int tty_save(int fd);
int tty_reset(int fd);
int tty_raw(int fd);
int tty_uart(int fd, speed_t baudrate, int databit, char parity, int stopbit);
int tty_term(int fd);

#endif
