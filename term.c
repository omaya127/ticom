#include <stdio.h>
#include "term.h"

struct termios save_opts[2];

int tty_save(int fd)
{
    struct termios opt;

    if (tcgetattr(fd, &opt) < 0) {
        return -1;
    }

    switch (fd) {
        case STDIN_FILENO:
            save_opts[0] = opt;
            break;
        default:
            save_opts[1] = opt;
            break;
    }

    return 0;
}

int tty_reset(int fd)
{
    struct termios opt;
    switch (fd) {
        case STDIN_FILENO:
            opt = save_opts[0];
            break;
        default:
            opt = save_opts[1];
            break;
    }
    if (tcsetattr(fd, TCSAFLUSH, &opt) < 0) {
        return -1;
    }
    return -1;
}

int tty_raw(int fd)
{
    struct termios opt;

    if (tcgetattr(fd, &opt) < 0) {
        return -1;
    }

    opt.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    opt.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    opt.c_cflag &= ~(CSIZE | PARENB);

    opt.c_cflag |= CS8;
    opt.c_oflag &= ~(OPOSE);
    opt.c_cc[VMIN] = 1;
    opt.c_cc[VTIME] = 0;

    if (tcsetattr(fd, TCSAFLUSH, &opt) < 0) {
        return -1;
    }

    /* Verify */
    if (tcgetattr(fd, &opt) < 0) {
        return -1;    
    }

    if ((opt.c_lflag & (ECHO | ICANON | IEXTEN | ISIG)) ||
            (opt.c_iflag & (BRKINT | ICRNL | INPCK | ISTRIP | IXON)) ||
            (opt.c_cflag & (CSIZE | PARENB | CS8) != CS8) ||
            (opt.c_oflag & (OPOSE)) || opt.c_cc[VMIN] != 1 || opt.c_cc[VTIME] != 0) {
        printf("verify failed!\n");
        return -1;
    }

    return 0;
}

int tty_set(int fd, int baudrate, int databit, char parity, int stopbit)
{
    struct termios opt;

    if (tcgetattr(fd, &opt) < 0) {
        return -1;
    }

    cfsetispeed(opt, baudrate);
    cfsetospeed(opt, baudrate);
    opt->c_cflag |= CS8; // databit
    opt->c_cflag &= ~PARENB; // no check bit

}
#if 0
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
#endif
