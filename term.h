#ifndef _TERM_H_
#define _TERM_H_

#include <termios.h>

struct termset {
    int fd;
    struct termios opt_old;
    struct termios opt_now;
};

int  tty_set_baudrate(struct termios *opt, unsigned int baudrate);
void tty_set_databit(struct termios *opt, unsigned int databit);
void tty_set_parity(struct termios *opt, char parity);
void tty_set_stopbit(struct termios *opt, const char *stopbit);

#endif
