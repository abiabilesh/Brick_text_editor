#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>
#include <brick_init.h>
#include <brick_termios.h>
#include <brick_exit.h>

extern Brick brick;

void terminos_raw_clear(void)
{
    if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &brick.org_term) == -1)
        die("terminos_raw_clear:tcsetattr");
}

void terminos_raw(Brick *brick)
{
    struct termios term;
    
    if(tcgetattr(STDIN_FILENO,&brick->org_term) == -1)
        die("terminos_raw:tcgetattr");
    atexit(terminos_raw_clear);
    
    term = brick->org_term;
    term.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
    term.c_oflag &= ~(OPOST);
    term.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    term.c_cflag |= (CS8);
    term.c_cc[VMIN] = 0;
    term.c_cc[VTIME] = 1;
    
    if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &term) == -1)
        die("terminos_raw:tcsetattr");
}

void get_terminos_property(Brick *brick)
{
    if(brick_get_window_size(&brick->brick_row, &brick->brick_column) == -1){
        die("get_terminos_property:brick_get_window_size");
    }
}
