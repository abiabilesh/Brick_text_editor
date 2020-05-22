#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <brick_core.h>

char* container_adder(brick_window *win, int *len)
{
    int iterator;
    char *adder;
    *len = 0;    
    row_container *container = win->container;

    for(iterator = 0; iterator < win->data_row; iterator++){
       *len += container[iterator].size + 1;
    }    
   
    adder = malloc(*len);
    char *c_iterator = adder;
    
    for(iterator = 0; iterator < win->data_row; iterator++){
        memcpy(c_iterator, container[iterator].data, container[iterator].size);
        c_iterator += container[iterator].size;
        *c_iterator = '\n';
        c_iterator ++;   
    }

    return adder;
}


int brick_file_save(brick_window *win)
{
   int fp, buffer_len;
   int ret = 0;
   char *filename = win->filename;
   
   char *file_buffer = container_adder(win, &buffer_len);
      

   if(filename == NULL)
       goto brick_file_save_exit;
  
   fp = open(filename, O_RDWR | O_CREAT, 0644);
   if(fp == NULL)
       goto brick_file_save_exit;
   
   if(ftruncate(fp,buffer_len) != 0)
       goto brick_file_save_exit;
   
   if(write(fp, file_buffer, buffer_len) != buffer_len)
       goto brick_file_save_exit;
   
   close(fp);
   free(file_buffer);
   return 0;   
   
   brick_file_save_exit:
       free(file_buffer);
       close(fp);
       return -1;   
}
