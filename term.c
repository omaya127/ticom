#include "term.h"
#include <stdio.h>
#include <stdlib.h>

struct termios save_opts[2];

int tty_save(int fd)
{
    struct termios opt;

    if (tcgetattr(fd, &opt) < 0) {
        return -1;
    }

    switch (fd) {
        case 0:
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
        case 0:
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

    if (tcgetattr(fd, &opt) < 0) 
        return -1;

    opt.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    opt.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    opt.c_cflag &= ~(CSIZE | PARENB);
    opt.c_cflag |= CS8;
    opt.c_oflag &= ~(OPOST);
    opt.c_cc[VMIN] = 1;
    opt.c_cc[VTIME] = 0;

    if (tcsetattr(fd, TCSAFLUSH, &opt) < 0)
        return -2;

    /* Verify */
    if (tcgetattr(fd, &opt) < 0)
        return -3;    

    if ((opt.c_lflag & (ECHO | ICANON | IEXTEN | ISIG)) ||
            (opt.c_iflag & (BRKINT | ICRNL | INPCK | ISTRIP | IXON)) ||
            (opt.c_cflag & (CSIZE | PARENB | CS8)) != CS8 ||
            (opt.c_oflag & (OPOST)) || 
            opt.c_cc[VMIN] != 1 || opt.c_cc[VTIME] != 0) {
        return -4;
    }

    return 0;
}

int tty_set(int fd, char *baudrate, char *databit, char *parity, char *stopbit)
{
    struct termios opt;

    if (tcgetattr(fd, &opt) < 0)
        return -1;

    speed_t speed;
    switch (atoi(baudrate)) {
        case 2400:
            speed = B2400; break;
        case 4800:
            speed = B4800; break;
        case 9600:
            speed = B9600; break;
        case 19200:
            speed = B19200; break;
        case 38400:
            speed = B38400; break;
        case 57600:
            speed = B57600; break;
        case 115200:
            speed = B115200; break;
        case 230400:
            speed = B230400; break;
        case 460800:
            speed = B460800; break;
        case 500000:
            speed = B500000; break;
        case 576000:
            speed = B576000; break;
        case 921600:
            speed = B921600; break;
        case 1000000:
            speed = B1000000; break;
        case 1152000:
            speed = B1152000; break;
        case 1500000:
            speed = B1500000; break;
        case 2000000:
            speed = B2000000; break;
        case 2500000:
            speed = B2500000; break;
        case 3000000:
            speed = B3000000; break;
        case 3500000:
            speed = B3500000; break;
        case 4000000: 
            speed = B4000000; break;
        default:
            speed = B115200; break;
    }
    cfsetispeed(&opt, speed);
    cfsetospeed(&opt, speed);

    switch (databit[0]) {
        case '5':
            opt.c_cflag |= CS5;
            break;
        case '6':
            opt.c_cflag |= CS6;
            break;
        case '7':
            opt.c_cflag |= CS7;
            break;
        case '8':
            opt.c_cflag |= CS8;
            break;
        default:
            opt.c_cflag |= CS8;
            break;
    }

    switch (parity[0]) {
        case 'N':
        case 'n':
            opt.c_cflag &= ~PARENB;
            break;
        case 'E':
        case 'e':
            opt.c_cflag |= PARENB;
            opt.c_cflag &= ~PARODD;
            break;
        case 'O':
        case 'o':
            opt.c_cflag |= PARENB;
            opt.c_cflag |= PARODD;
            break;
        default:
            opt.c_cflag &= ~PARENB;
            break;
    }

    switch (stopbit[0]) {
        case '1':
            opt.c_cflag &= ~CSTOPB;
            break;
        case '2':
            opt.c_cflag |= CSTOPB;
            break;
        default:
            opt.c_cflag &= ~CSTOPB;
            break;
    }

    if (tcflush (fd, TCIFLUSH) < 0)
        return -2;

    if (tcsetattr(fd, TCSANOW, &opt) < 0)
        return -3;

    return 0;
}
int tty_std(int fd)
{
    struct termios opt;
    if (tcgetattr(fd, &opt) < 0) {
        return -1;
    }
    opt.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    opt.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    if (tcsetattr(fd, TCSAFLUSH, &opt) < 0) {
        printf("\n[!] tty set term failed!\n");
        return -1;
    }
    return 0;
}
