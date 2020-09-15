#ifndef define_termios_manager
#define define_termios_manager

typedef struct termios_ioctl_functions termios_ioctl_functions; 
typedef struct termios_functions termios_functions;
typedef struct termios_manager termios_manager;


#include <stdbool.h>
#include <termios.h>
#include <termios_interface.h>

struct termios_ioctl_functions
{
    int(* get_window_size)(int *rows, int *cols);   //function getting the window size
};

struct termios_functions
{
    struct termios* (*set_termios_raw)(struct termios original_termios, bool *is_termios_raw);    //function setting the termios raw
    int (*set_termios_original)(struct termios original_termios, bool *is_termios_raw);           //function setting the termios original
    termios_ioctl_functions *termios_ioctl_fn;                                                    //contains termios_ioctl_functions
};


struct termios_manager
{
    bool is_termios_raw;                //flags for raw_terminal status
    struct termios modified_termios;    //contains the modified termios structure
    struct termios original_termios;    //contains the original termios structure
    termios_functions *termios_func;    //contains the functions implemented by termios_manager
    termios_interface *termios_intf;    //structure to expose the interface to other module
};


int termios_manager_init(termios_manager *termios_mgr);
int termios_manager_exit(termios_manager *termios_mgr);

#endif