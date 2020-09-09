#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <file_manager.h>

char* container_adder(file_container *container, int *len)
{
    int iterator;
    char *adder;
    *len = 0;    
    line_container *l_container = container->file_rows;

    for(iterator = 0; iterator < container->file_rows; iterator++){
       *len += l_container[iterator].size + 1;
    }    
   
    adder = malloc(*len);
    char *c_iterator = adder;
    
    for(iterator = 0; iterator < container->file_rows; iterator++){
        memcpy(c_iterator, l_container[iterator].data, l_container[iterator].size);
        c_iterator += l_container[iterator].size;
        *c_iterator = '\n';
        c_iterator ++;   
    }

    return adder;
}


int editor_file_save(file_container *container, char *file_name)
{
   int fp, buffer_len;
   int ret = 0;
   char *filename = file_name;
   
   char *file_buffer = container_adder(container, &buffer_len);
      
    if(filename == NULL)
       goto brick_file_save_exit;
  
    fp = open(filename, O_RDWR | O_CREAT, 0644);
    if(fp == NULL){
        //have to add debug api's   
        goto brick_file_save_exit;
    }

    if(ftruncate(fp,buffer_len) != 0){
        //have to add debug api's   
        goto brick_file_save_exit;
    }
   
    if(write(fp, file_buffer, buffer_len) != buffer_len){
        //have to add debug api's   
        goto brick_file_save_exit;
   }   
    
    close(fp);
    free(file_buffer);
    return ret;   
   
   brick_file_save_exit:
       free(file_buffer);
       close(fp);
       ret = -1;
       return ret;   
}