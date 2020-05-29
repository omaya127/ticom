#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "parse.h"
#include "term.h"


int main(int argc, char *argv[])
{
    int tty_fd = -1;
    struct termset term_std, term_tty;
    //struct timeval *ptv;
    fd_set rset, wset;
    int ret, nbytes;

    tty_fd = open(argv[1], O_RDWR | O_NONBLOCK | O_NOCTTY);
    if (tty_fd < 0) {
        printf("open tty failed!\n"); 
        return -1;
    }

    /* set tty */
    if (tcgetattr(tty_fd, &term_tty.old_opt) < 0) {
        printf("[!] tcgetattr failed!\n");
    }
    term_tty.now_opt = term_tty.old_opt;
    tty_set_baudrate(&term_tty.now_opt, 115200);
    tty_set_databit(&term_tty.now_opt, 8);
    tty_set_parity(&term_tty.now_opt, 'N');
    tty_set_stopbit(&term_tty.now_opt, NULL);
    if (tcflush (tty_fd, TCIFLUSH) < 0) {
        printf("[!] tcflush failed\n");
    }
    if (tcsetattr(tty_fd, TCSANOW, &term_tty.now_opt) < 0) {
        printf("[!] tcsetattr failed\n");
    }

    tcgetattr(tty_fd, &term_tty.now_opt);

    /* set stdin */
    tcgetattr(STDIN_FILENO, &term_std.old_opt);
    term_std.now_opt = term_std.old_opt;
    term_std.now_opt.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    term_std.now_opt.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &term_std.now_opt);

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
                switch (parse_keys(buf, nbytes)) {
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
                printf("read stdin failed\n");
                goto exit_label;
            }
        }

        /* read from tty */
        if (FD_ISSET(tty_fd, &rset)) {
            char buf[128];
            nbytes = read(tty_fd, buf, 128);
            if (nbytes <= 0) {
                printf("[!] read tty failed\n");
                goto exit_label;
            }
            write(STDOUT_FILENO, buf, nbytes);
        }
    }

exit_label:
    tcsetattr(tty_fd, TCSANOW, &term_tty.old_opt);
    tcsetattr(STDIN_FILENO, TCSANOW, &term_std.old_opt);
    close(tty_fd);
    printf("\n[-] ticom exit\n");
    return 0;
}
