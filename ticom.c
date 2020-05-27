#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>

int tty_fd = -1;

int main(int argc, char *argv[])
{
    struct termios term;

    tty_fd = open(argv[1], O_RDWR | O_NONBLOCK | O_NOCTTY);
    if (tty_fd < 0) {
        printf("open tty failed!\n"); 
        return -1;
    }

    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &term);

    tcgetattr(tty_fd, &term);
    // baud
    cfsetispeed(&term, 115200);
    cfsetospeed(&term, 115200);
    // data bit
    term.c_cflag &= ~CSIZE;
    term.c_cflag |= CS8;
    // check bit
    term.c_cflag &= ~PARENB;
    // stop bit
    term.c_cflag &= ~CSTOPB;
    tcflush (tty_fd, TCIFLUSH);
    tcsetattr(tty_fd, TCSANOW, &term);

    //struct timeval *ptv;
    int ret, nbytes;
    fd_set rset, wset;

    for (;;) {
        FD_ZERO(&rset);
        FD_ZERO(&wset);
        FD_SET(tty_fd, &rset);
        FD_SET(STDIN_FILENO, &rset);

        ret = select(tty_fd + 1, &rset, NULL, NULL, NULL);

        if (ret == -1) {
            printf("select error\n");
            return -1;
        }

        /* read from term */
        if (FD_ISSET(STDIN_FILENO, &rset)) {
            char buf[32];
            nbytes = read(STDIN_FILENO, buf, 32);
            if (nbytes > 0) {
                write(tty_fd, buf, nbytes);
            }
        }
        if (FD_ISSET(tty_fd, &rset)) {
            char buf[128];
            nbytes = read(tty_fd, buf, 128);
            if (nbytes > 0) {
                write(STDOUT_FILENO, buf, nbytes);
            }
        }
    }
}
