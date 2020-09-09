#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>
#include <stdlib.h>
#include <termios_interface.h>

/*
 * Function: termios_interface_init 
 * --------------------
 * Registers the interface to be exposed to other modules 
 * by filling the termios manager structure
 *
 * termios_manager* structure of termios manager to be filled 
 * with
 *
 * returns: returns -1 on error (if malloc fails) \
 * or 0 on success
 * 
 */
int termios_interface_init(termios_manager* termios_mgr)
{
    int ret = 0;
    termios_interface interface;

    if(termios_mgr == NULL){
         //have to add debug api's
         goto termios_interface_init_fail;
    }

    if(termios_mgr->termios_func->termios_ioctl_fn == NULL){
        //have to add debug api's
         goto termios_interface_init_fail;
    }

    interface.get_window_size = termios_mgr->termios_func->termios_ioctl_fn;
    termios_mgr->termios_intf = &interface; //Assigning the interface to terminal manager

    return ret;

termios_interface_init_fail:
    ret = -1;
    return ret;
}

/*
 * Function: termios_interface_exit 
 * --------------------
 * Frees the termios_interface structure 
 *
 * termios_interface* the structure to be cleared during 
 * application exit
 *
 * returns: returns -1 on error (if malloc fails) \
 * or 0 on success
 * 
 */
int termios_interface_exit(termios_interface* interface)
{
    int ret = 0;
    if(interface == NULL){
        return ret;
    }

     //have to add debug api's
    free(interface);
    return ret;
}
