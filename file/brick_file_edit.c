#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <brick_core.h>
#include <brick_file.h>

void container_row_change(struct brick_win_size *win, int character)
{
    
    row_container *container = &win->container[win->current_row];
    int offset = win->current_column;
    char *data = win->container[win->current_row].data;
    if(offset < 0 )
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

void container_add_row(struct brick_win_size *win, char *str, int len)
{
    int row = win->current_row;
    win->container = realloc(win->container, sizeof(row_container) * (row + 1));
    row_container *container = win->container;
   
    container[row].data = malloc(len+1);
    memcpy(container[row].data,str,len);
    container[row].data[len] = '\0';
   
    container[row].size = len;
    win->data_row ++;
}

void container_insert_character(struct brick_win_size *win, int character)
{  
    if(win->current_row != win->data_row)
        container_row_change(win, character);
    else 
        container_add_row(win,"",0);
}
