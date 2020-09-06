#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios_manager.h>

 //termios manager global structure

/*
 * Function: termios_get_window_fallback 
 * --------------------
 * Assigns an default value incase the tty driver does not
 * support get window IOCTL calls
 *
 * *row  : pointer to an integer to fill the no of rows
 * *cols : pointer to an integer to fill the no of columns
 *
 * returns: returns -1 on error (if malloc fails) \
 * or 0 on success
 * 
 */

int termios_get_window_fallback(int *rows, int *cols)
{
    unsigned int i = 0;
    int ret = 0;
    char buffer[32];
    
    if(write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12)!=12){    
        //have to add debug api's
        ret = -1;
        goto termios_get_window_fallback_fail;
    }

    if(write(STDOUT_FILENO, "\x1b[6n",4)!=4){    
        //have to add debug api's
        ret = -1;
        goto termios_get_window_fallback_fail;
    }

    while(i < sizeof(buffer) - 1)
    {  
        if(read(STDIN_FILENO,&buffer[i],1) != 1 )
            break;
        if(buffer[i] == 'R')
            break;
        i++;
    }
    
    buffer[i]='\0';
    if(buffer[0]!='\x1b' || buffer[1] != '['){    
        //have to add debug api's
        ret = -1;
        goto termios_get_window_fallback_fail;
    }

    if(sscanf(&buffer[2], "%d;%d", rows, cols) != 2){    
        //have to add debug api's
        ret = -1;
        goto termios_get_window_fallback_fail;
    }

    return 0;

termios_get_window_fallback_fail:
    return ret;

}

/*
 * Function: get_window_size 
 * --------------------
 * Uses IOCTL call to tty driver to retrieve the rows and columns
 * size of an window
 *
 * *row  : pointer to an integer to fill the no of rows
 * *cols : pointer to an integer to fill the no of columns
 *
 * returns: returns -1 on error (if malloc fails) \
 * or 0 on success
 * 
 */

int get_window_size(int *rows, int *cols)
{
    int ret = 0;
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) 
    {
        ret = termios_get_window_fallback(rows,cols);
        if(ret)
            {
                //have to add debug api's
                return -1;
            }
        return 0;
    } 
    else 
    {
        *cols = ws.ws_col;
        *rows = ws.ws_row;
        return 0;
    }
}

/*
 * Function: set_termios_raw 
 * --------------------
 * Sets the termios structure into raw format for
 * text editor mode
 *
 * original_termios: Requires the original termios structure \
 * for setting raw mode
 * is_termios_raw: sets the status of the terminal 
 *
 * returns: returns newly modified struct termios* on success \
 * if not NULL on error
 * 
 */

struct termios* set_termios_raw(struct termios original_termios, bool *is_termios_raw)
{
    int ret = 0;
    struct termios modified_termios;

    modified_termios.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
    modified_termios.c_oflag &= ~(OPOST);
    modified_termios.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    modified_termios.c_cflag |= (CS8);
    modified_termios.c_cc[VMIN] = 0;
    modified_termios.c_cc[VTIME] = 1;

    ret = tcsetattr(STDIN_FILENO, TCSAFLUSH, &modified_termios);
    if(ret < 0){
        //have to add debug api's
        return NULL;
    }

    *is_termios_raw = true;

    return &modified_termios;    
}


/*
 * Function: set_termios_original 
 * --------------------
 * Sets the termios structure into normal mode during exit
 *
 * original_termios: Requires the original termios structure \
 * for setting the termios mode
 * is_termios_raw: sets the status of the terminal 
 * 
 * returns: returns -1 on error (if malloc fails) \
 * or 0 on success
 * 
 */

int set_termios_original(struct termios original_termios, bool *is_termios_raw)
{
    int ret = 0;

    ret = tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios);
    *is_termios_raw = false;
    
    if(ret < 0){
        *is_termios_raw = true;
        //have to add debug api's
    }
    
    return ret;   
}


/*
 * Function: termios_manager_init 
 * --------------------
 * Intializes the global Termios_manager structure
 *
 * termios_manager* the termios manager structure to be 
 * filled with  
 *
 * returns: returns -1 on error (if malloc fails) \
 * or 0 on success
 * 
 */

int termios_manager_init(termios_manager *termios_mgr)
{
    int ret = 0;
    termios_manager *manager;
    termios_functions *function;
    termios_ioctl_functions *ioctl_function;
    termios_interface *interface;

    manager = (termios_manager*) malloc(sizeof(termios_manager));
    if(manager == NULL){
        //have to add debug api's      
        goto termios_manager_init_fail;
    }

    function = (termios_functions*)malloc(sizeof(termios_functions));
    if(function == NULL){
        //have to add debug api's  
        goto termios_manager_init_fail;
    }

    function->set_termios_raw      = &set_termios_raw;
    function->set_termios_original = &set_termios_original;

    ioctl_function = (termios_ioctl_functions*)malloc(sizeof (termios_ioctl_functions));
    if(ioctl_function == NULL){
        //have to add debug api's
        goto termios_manager_init_fail;
    }

    ioctl_function->get_window_size = &get_window_size;
    function->termios_ioctl_fn = ioctl_function;

    manager->termios_func = function;
    
    interface = (termios_interface*) malloc(sizeof(termios_interface));
    if(interface == NULL){
        //have to add debug api's
        goto termios_manager_init_fail;
    }
    
    manager->termios_intf = interface;  //Intializing the interface of termios manager
    ret = termios_interface_init(manager);
    if(ret == 0){
        //have to add debug api's
        goto termios_manager_init_fail;
    }
    
    termios_mgr = manager;          //Filling the termios manager structure

    //have to add debug api's
    return ret;

termios_manager_init_fail:
    ret = -1;   
    return ret;
}


/*
 * Function: termios_manager_exit 
 * --------------------
 * Free's the global Termios_manager structure
 *
 * termios_manager* structure to be freed
 *
 * returns: returns -1 on error (if malloc fails)
 */

int termios_manager_exit(termios_manager *termios_mgr)
{
    int ret = 0;

    termios_manager *manager                    = termios_mgr;
    termios_functions *function                 = termios_mgr->termios_func;
    termios_ioctl_functions *ioctl_function     = function->termios_ioctl_fn;
    termios_interface *interface                = termios_mgr->termios_intf;

    free(ioctl_function);
    free(function);
    
    ret = termios_interface_exit(interface);

    free(manager);
    //have to add debug api's
    return ret;
}
