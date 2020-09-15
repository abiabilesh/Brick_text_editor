#ifndef CORE_MANAGER
#define CORE_MANAGER

#include <termios_manager.h>
#include <file_manager.h>

typedef struct core_manager core_manager; 

struct core_manager
{    
    file_manager *core_file_mgr;             //structure to access the file manager
    termios_interface *core_termios_intf;    //structure to access the termios interface 
};


//Key related variables

#define CTRL_KEY(k) (k & 0x1f)

enum editor_key{
    BACKSPACE = 127,
    DEL_KEY,
    ARROW_UP = 1000,
    ARROW_DOWN,
    ARROW_LEFT,
    ARROW_RIGHT,
    ARROW_PG_UP,
    ARROW_PG_DOWN,
    ARROW_HOME,
    ARROW_END
};

int core_manager_init(core_manager *core_mgr);
int core_manager_exit(core_manager *core_mgr);

#endif //CORE_MANAGER