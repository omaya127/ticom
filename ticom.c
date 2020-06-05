#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "keymap.h"
#include "term.h"


int main(int argc, char *argv[])
{
    int tty_fd = -1;
    //struct timeval *ptv;
    fd_set rset, wset;
    int ret, nbytes;

    tty_fd = open(argv[1], O_RDWR | O_NONBLOCK | O_NOCTTY);
    if (tty_fd < 0) {
        printf("\n[!] open tty failed!\n"); 
        return -1;
    }

    ret = tty_raw(tty_fd);
    if (ret < 0) {
        printf("\n[!] tty raw failed: %d!\n", ret);
        goto exit_label;
    }

    ret = tty_set(tty_fd, "115200", "0", "0", "0");
    if (ret < 0) {
        printf("\n[!] tty set failed: %d!\n", ret);
        goto exit_label;
    }

    tty_save(STDIN_FILENO);
    tty_std(STDIN_FILENO);

    for (;;) {
        FD_ZERO(&rset);
        FD_ZERO(&wset);
        FD_SET(tty_fd, &rset);
        //FD_SET(tty_fd, &wset);
        FD_SET(STDIN_FILENO, &rset);

        ret = select(tty_fd + 1, &rset, &wset, NULL, NULL);

        if (ret == -1) {
            printf("\n[!] select error\n");
            return -1;
        }

        /* read from stdin */
        if (FD_ISSET(STDIN_FILENO, &rset)) {
            char buf[32];
            nbytes = read(STDIN_FILENO, buf, 32);
            if (nbytes > 0) {
                switch (keymap(buf, nbytes)) {
                    case KEY_CTRL_A:
                        break;
                    case KEY_CTRL_Q:
                        goto exit_label;
                        break;
                    default:
                        write(tty_fd, buf, nbytes);
                        break;
                }
            }
            else{
                printf("\n[!] read stdin failed\n");
                goto exit_label;
            }
        }

        /* read from tty */
        if (FD_ISSET(tty_fd, &rset)) {
            char buf[128];
            nbytes = read(tty_fd, buf, 128);
            if (nbytes <= 0) {
                printf("\n[!] read tty failed\n");
                goto exit_label;
            }
            write(STDOUT_FILENO, buf, nbytes);
        }
    }

exit_label:
    tty_reset(STDIN_FILENO);
    close(tty_fd);
    printf("\n[-] ticom exit\n");
    return 0;
}
