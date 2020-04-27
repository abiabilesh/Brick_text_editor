#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>
#include <brick_init.h>
#include <brick_core.h>
#include <brick_termios.h>
#include "brick_screen.h"

Brick brick;

int main(int argc, char *argv[])
{
    int screen_thread;
    terminos_raw(&brick);
    get_terminos_property(&brick);
    brick_core_init(brick);
    screen_thread = brick_screen_init();

    brick_refresh_screen(); //update the screen once and let screen_thread \
                             take over from next time    
    while(1)
    {
        brick_core_inloop();
    }

    pthread_join(screen_thread, NULL);
 
    return 0;
}
