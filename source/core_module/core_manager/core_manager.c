#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <core_manager.h>

int read_user_command(void)
{
    int ret;
    char command = '\0';
    while(ret = read(STDIN_FILENO,&command,1) != 1)
    {
        if((ret== -1) && (errno != EAGAIN))
        {
            //have to add debug api's and exit application
            exit(-1);
        }
    }
    
    if(command == '\x1b')
    {
        char str[3];
        if(read(STDIN_FILENO,&str[0],1) != 1)
            return '\x1b';
        if(read(STDIN_FILENO,&str[1],1) != 1)
            return '\x1b';
        if(str[0] == '[')
        {
            if(str[1] >= '1' && str[1] <= '9')
            {
                if(read(STDIN_FILENO,&str[2],1) != 1)
                    return '\x1b';
                if(str[2] == '~')
                {
                    switch(str[1])
                    {
			case '3':return DEL_KEY;
                        case '1':
                        case '7':return ARROW_HOME;
		    	case '4':return ARROW_END;
                        case '5':return ARROW_PG_UP;
                        case '6':return ARROW_PG_DOWN;
                    }
                }
                else 
                    return '\x1b';
            }
            else
            {
                switch(str[1]){
                    case 'A': return ARROW_UP;
                    case 'B': return ARROW_DOWN;
                    case 'C': return ARROW_RIGHT;
                    case 'D': return ARROW_LEFT;
                    case 'H': return ARROW_HOME;
                }
            }
        }
        return '\x1b';    
    }
    else
    {
        return command;
    }
}


void core_input_processor(void)
{
    int input_char;

    input_char = read_user_command();
    switch(input_char)
    {
        case CTRL_KEY('l'):
            /* TODO */
            break;

        case CTRL_KEY('h'):
            /* TODO */
            break;

        case CTRL_KEY('q'):
            break;
        case CTRL_KEY('s'):  
            break;
        case ARROW_UP:
        case ARROW_DOWN:
        case ARROW_LEFT:
        case ARROW_RIGHT:
            break;
            
        case ARROW_HOME:
            break;
        
        case ARROW_END:
            break;
    
        case ARROW_PG_UP:
        case ARROW_PG_DOWN:
            break; 
        
        case BACKSPACE:
            break;
        case DEL_KEY:   
            break;

        case '\r':
            /* TODO */
            break;

        default:
            break;
    }
}


int core_manager_init(core_manager *core_mgr)
{
    int ret = 0;
    file_manager *file_mgr;

    //Intializing the file manager 
    ret = file_manager_init(file_mgr);
    if(ret != 0){
        //have to add debug api's   
        goto core_manager_init_fail;
    }

    //Has to be intialized by editor_init
    if(core_mgr->core_termios_intf == NULL)
    {
        //have to add debug api's   
        ret = -1;
        goto core_manager_init_fail;
    } 

core_manager_init_fail:
    return ret;
}

int core_manager_exit(core_manager *core_mgr)
{
    int ret = 0;
    file_manager *file_mgr = core_mgr->core_file_mgr;
    termios_interface *term_intf = core_mgr->core_termios_intf;

    ret = file_manager_exit(file_mgr);
    if(ret != 0){
        //have to add debug api's   
        ret = -1;
        goto core_manager_exit_fail;
    }

    free(term_intf);

core_manager_exit_fail:
    return ret;
}