#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int tty_fd = -1;
int stdin_fd = 0;
int stdout_fd = 1;

int main(int argc, char *argv[])
{
    tty_fd = open(arvg[1], O_RDWR | O_NONBLOCK | O_NOCTTY);
    if (tty_fd < 0) {
        printf("open tty failed!\n"); 
        return -1;
    }

    struct timeval *ptv;
    for (;;) {
        FD_ZERO(&rset);
        FD_ZERO(&wset);
        FD_SET()
    }
}
