#include <stdio.h>
#include "term.h"

int tty_set_baudrate(struct termios *opt, unsigned int baudrate)
{
    cfsetispeed(opt, baudrate);
    cfsetospeed(opt, baudrate);
}

void tty_set_databit(struct termios *opt, unsigned int databit)
{
    opt->c_cflag &= ~CSIZE;
    switch (databit) {
        case 8:
            opt->c_cflag |= CS8;
            break;
        case 7:
            opt->c_cflag |= CS7;
            break;
        case 5:
            opt->c_cflag |= CS5;
            break;
        case 6:
            opt->c_cflag |= CS6;
            break;
        default:
            opt->c_cflag |= CS8;
            break;
    }
}

void tty_set_parity(struct termios *opt, char parity)
{
    switch (parity) {
        case 'N':                  /* no parity check */
            opt->c_cflag &= ~PARENB;
            break;
        case 'E':                  /* even */
            opt->c_cflag |= PARENB;
            opt->c_cflag &= ~PARODD;
            break;
        case 'O':                  /* odd */
            opt->c_cflag |= PARENB;
            opt->c_cflag |= ~PARODD;
            break;
        default:                   /* no parity check */
            opt->c_cflag &= ~PARENB;
            break;
    }
}

void tty_set_stopbit(struct termios *opt, const char *stopbit)
{
    opt->c_cflag &= ~CSTOPB; /* 1 stop bit */
    //opt->c_cflag |= CSTOPB;  /* 2 stop bits */
}
#if 0
int term_set_raw(int fd, struct termios *opt)
{
    //tcgetattr(terms->fd, opt);
    opt->c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    opt->c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    tcsetattr(fd, TCSAFLUSH, opt);
}

int term_set_opt(int fd, struct termios *opt)
{
    tcflush (fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &term);
}
#endif
