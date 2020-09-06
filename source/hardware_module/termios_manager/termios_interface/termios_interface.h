#ifndef define_termios_interface
#define define_termios_interface

typedef struct termios_interface termios_interface; 

#include <termios_manager.h>

struct termios_interface
{
    int(* get_window_size)(int *rows, int *cols);   //function getting the window size
};

int termios_interface_init(termios_manager* termios_mgr);
int termios_interface_exit(termios_interface* interface);

#endif