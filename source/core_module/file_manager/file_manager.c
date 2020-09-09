#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <file_manager.h>
#include <file_functions.h>

int file_manager_init(file_manager *file_mgr)
{
    int ret = 0;
    file_manager *manager;
    file_functions *function;
    file_container *container;
    line_container *l_container;

    manager = (file_manager*) malloc(sizeof(file_manager));
    if(manager == NULL){
        //have to add debug api's      
        goto file_manager_init_fail;
    }

    function = (file_functions*) malloc(sizeof(file_functions));
    if(function == NULL){
        //have to add debug api's      
        goto file_manager_init_fail;
    }

    manager->file_name = NULL;

    function->file_open = &editor_file_open;
    function->file_edit = NULL;
    function->file_save = &editor_file_save;
    function->file_close = NULL;

    manager->file_fnc = function;

    l_container = NULL;
    container->lin_con = l_container;
    container->file_rows = NULL;

    manager->container = container;

    file_mgr = manager;     //Intializing the file manager structure

file_manager_init_fail:
    ret = -1;   
    return ret;
}


int file_manager_exit(file_manager *file_mgr)
{
    int iterator;
    int ret = 0;
    file_manager *manager = file_mgr;
    file_functions *function = manager->file_fnc;
    file_container *container = manager->file_fnc;
    line_container *l_container = container->lin_con;

    //Freeing the l_containers
    for(iterator = 0; iterator < container->file_rows ; iterator++)
        free(&l_container[iterator]);
    
    //Freeing the container
    free(container);

    //Freeing the function pointers
    function->file_open = NULL;
    function->file_edit = NULL;
    function->file_save = NULL;
    function->file_close = NULL;

    //Freeing the manager
    free(manager);

    //have to add debug api's   
    return ret;
}