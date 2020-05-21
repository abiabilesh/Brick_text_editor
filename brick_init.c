#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>
#include <stdarg.h>
#include <brick_init.h>
#include <brick_core.h>
#include <brick_termios.h>
#include "brick_screen.h"

Brick brick;

void brick_set_mbar(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(brick.message_bar, sizeof(brick.message_bar), fmt, ap);
    va_end(ap);
    brick.message_time = time(NULL);
}


void brick_main(Brick *brick)
{
    brick->brick_row = 0;
    brick->brick_column = 0;
    brick->message_bar[0] = '\0';
    brick->message_time = 0;  
    brick_mbar_init(&brick);  
}


void brick_mbar_init(Brick *brick)
{
    brick_set_mbar("HELP: CTRL+Q = QUIT");
}


int main(int argc, char *argv[])
{
    int screen_thread;

    brick_main(&brick);
    terminos_raw(&brick);
    get_terminos_property(&brick);
    
    if (argc == 2) 
        brick_core_init(brick, argv[1]);
    else{
        printf("[brick]: Please provide a file to open\n\r");
        return -1;        
    }

    screen_thread = brick_screen_init();
    
    while(1)
    {
        brick_core_inloop();
    }

    pthread_join(screen_thread, NULL);
 
    return 0;
}
