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

    if (tcgetattr(fd, &opt) < 0) {
        printf("\n[!] tty get raw\n");
        return -1;
    }

    opt.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    opt.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    opt.c_cflag &= ~(CSIZE | PARENB);
    opt.c_cflag |= CS8;
    opt.c_oflag &= ~(OPOST);
    opt.c_cc[VMIN] = 1;
    opt.c_cc[VTIME] = 0;

    if (tcsetattr(fd, TCSAFLUSH, &opt) < 0) {
        printf("\n[!] tty set raw\n");
        return -1;
    }

    /* Verify */
    if (tcgetattr(fd, &opt) < 0) {
        printf("\n[!] tty get raw2\n");
        return -1;    
    }

    if ((opt.c_lflag & (ECHO | ICANON | IEXTEN | ISIG)) ||
            (opt.c_iflag & (BRKINT | ICRNL | INPCK | ISTRIP | IXON)) ||
            (opt.c_cflag & (CSIZE | PARENB | CS8)) != CS8 ||
            (opt.c_oflag & (OPOST)) || 
            opt.c_cc[VMIN] != 1 || opt.c_cc[VTIME] != 0) {
        printf("\n[!] tty verify failed\n");
        return -1;
    }

    return 0;
}

int tty_uart(int fd, speed_t baudrate, int databit, char parity, int stopbit)
{
    struct termios opt;

    if (tcgetattr(fd, &opt) < 0) {
        return -1;
    }

    cfsetispeed(&opt, B115200); // baudrate
    cfsetospeed(&opt, B115200);
    opt.c_cflag |= CS8; // databit
    opt.c_cflag &= ~PARENB; // no parity check
    opt.c_cflag &= ~CSTOPB; // 1 stop bit
    if (tcflush (fd, TCIFLUSH) < 0) {
        printf("[!] tcflush failed\n");
        return -1;
    }
    if (tcsetattr(fd, TCSANOW, &opt) < 0) {
        printf("[!] tcsetattr failed\n");
        return -1;
    }
    return 0;
}
int tty_term(int fd)
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
