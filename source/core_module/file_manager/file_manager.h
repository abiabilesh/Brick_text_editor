#ifndef file_manager_header
#define file_manager_header

typedef struct file_functions file_functions;
typedef struct file_manager file_manager;
typedef struct file_container file_container;
typedef struct line_container line_container;

struct file_functions
{
    int (*file_open)(file_container *container, char *file_name, int scr_max_column);
    int (*file_edit)(int *file_descriptor);
    int (*file_save)(int *file_descriptor);
    int (*file_close)(int *file_descriptor);
};

struct line_container
{
    int size;       //character length of each line including null terminator
    char *data;     //string of each line including null terminator at the end
};

struct file_container
{
    line_container *lin_con; 
    int file_rows;          //integer holding the number of rows
};

struct file_manager
{
    char *file_name;            //contains the name of file editor opened
    file_container *container;  //structure for containing the contents of the file
    file_functions *file_fnc;   //contains the functions implemented by file_manager
};

int file_manager_init(file_manager *file_mgr);
int file_manager_exit(file_manager *file_mgr);

#endif


