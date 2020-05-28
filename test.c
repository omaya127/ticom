#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "parse.h"

int tty_fd = -1;

int main(int argc, char *argv[])
{
    struct termset term_std, term_tty;

    tty_fd = open(argv[1], O_RDWR | O_NONBLOCK | O_NOCTTY);
    if (tty_fd < 0) {
        printf("open tty failed!\n"); 
        return -1;
    }

    tcgetattr(STDIN_FILENO, &term_std.opt_old);
    term_std.opt_now = term_std.opt_old;
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
                if(parse_keys(buf, nbytes) == KEY_CTRL_Q) {
                    break;
                }
                write(STDIN_FILENO, buf, nbytes);
                printf(" ");
                for(int i=0; i<nbytes; i++){
                    printf("%02X", buf[i]);
                }
                printf("\n");
            }
        }
    }
    printf("[>] ticom exit\n");
    return 0;
}
