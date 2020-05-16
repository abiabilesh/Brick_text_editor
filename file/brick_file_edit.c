#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <brick_core.h>

void container_row_change(struct brick_win_size *win, int character)
{
    row_container *container = &(win->container[win->current_row]);
    int offset = win->current_column;
    char *data = win->container[win->current_row].data;
    if(offset < 0 || offset >= win->col)
        offset = container->size;

    data = realloc(data, win->container[win->current_row].size + 2 );
    win->container[win->current_row].data = data;
    memmove(&data[offset + 1], &data[offset], (win->container[win->current_row].size  - offset + 1));
    data[offset] = character;
    win->current_column++;
    container->size ++;
   
   /*
   int offset = win->current_column;
   if(offset < 0 || offset >= win->col)
        offset = container->size;
   win->container[win->current_row].data = realloc(win->container[win->current_row].data,\
       win->container[win->current_row].size + 2); 
   char *data = win->container[win->current_row].data;
   memmove(&data[offset + 1], &data[offset], (win->container[win->current_row].size  - offset + 1));
   data[offset] = character;
   win->current_column++;
   win->container[win->current_row].size ++;
   */
}

void container_add_row(struct brick_win_size *win, char *s, int len)
{

    int total_row = win->data_row; 
    row_container *container = win->container;
    
    container = realloc(container, sizeof(row_container) * (total_row + 1));
    total_row++;
    
    container[total_row].size = len;
    container[total_row].data = malloc(len+1);
    memcpy(container[total_row].data, s, len);

    win->data_row = total_row;
}


void container_insert_character(struct brick_win_size *win, int character)
{  
    if(win->current_row > win->data_row)
        container_add_row(win, "", 0);      

    container_row_change(win, character);
}
