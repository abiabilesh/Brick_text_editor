#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <stdlib.h>
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


void process_delete_key(struct brick_win_size *win)
{
    uint8_t insert_flag = 0;
    int row = win->current_row;
    int col = win->current_column;
    row_container *container = win->container;
    char *data = container[row].data;

/*
    FILE *fp;
    fp=fopen("log.txt","a+");

    fprintf(fp,"begin: row:%d, column:%d, row_size:%d, data_row:%d, data:%s\n", row, col, container[row].size, win->data_row, container[row].data);
*/

    /*************************************************************
     NOTE: container[row].size represents the total element in an 
     array so when it means 20, then there are 20 elements in that 
     container starting from 0 to 19. please evaluate col accordingly
     ie col is always 1 for 2nd element. Moreover we all ways have 
     null terminator as the final index..please note.
    *************************************************************/

    //check whether should we delete inline
    
    if((col < (container[row].size - 1)) && (row < win->data_row )){ 
        //fprintf(fp,"we delete inline\n");
        data = (char*) realloc (data, ((container[row].size) - 1) * sizeof(char*));
        memmove(&data[col], &data[col+1], container[row].size - (col + 1));
        //fprintf(fp,"data:%s size:%d\n",data,strlen(data)+1);
        container[row].data = data;
        container[row].size = strlen(data) + 1; //to include the null terminator
    }

    //check whether should we delete next line and append, also make sure it is not the last line
    
    else if((col == container[row].size - 1) && (row < win->data_row - 1)){
        //fprintf(fp,"---check whether should we delete next line and append, also make sure it is not the last line--\n");
        
        //check if the current line is empty
        
        if(container[row].size  == 1){  //just a null terminator          
            //fprintf(fp,"---the current line is empty--\n");
            
            //check whether the next line is empty
            if(container[row + 1].size == 0){
                //fprintf(fp,"---the next line is empty--\n");
                goto process_delete_next_line;
            }else{
                //fprintf(fp,"---the next line is not empty--\n");
                char *next_data = container[row + 1].data;
                data = (char*) realloc (data, (container[row + 1].size) * sizeof(char*));
                memcpy(data, next_data, container[row+1].size);
                container[row].data = data;
                container[row].size = container[row+1].size;
            }
        }else{
            //fprintf(fp,"---current line is not empty--\n");
            //check whether the next line is empty
            if(container[row + 1].size == 1){
                //fprintf(fp,"---the next line is empty--\n");
                goto process_delete_next_line;
            }else{
                 //fprintf(fp,"---the next line is not empty--\n");
                 char *next_data = container[row + 1].data;
                 data = (char*) realloc (data, (container[row].size + container[row + 1].size - 1) * sizeof(char*));
                 memcpy(&data[container[row].size - 1], next_data, container[row + 1].size);
                 container[row].data = data;
                 container[row].size = strlen(data) + 1;
            }             
        }    
        
        process_delete_next_line:
            //removing the next line and patching the containers
            for(int index = row + 1; index < win->data_row - 1; index++ )
            {
                container[index] = container[index + 1];
            }
            container = realloc (container, sizeof(row_container) * (win->data_row -1));
            win->container = container;
            win->data_row --;
            //fprintf(fp, "adding new row size %d and data %s\n", strlen(data)+1, data);
    }
 
    //We have reached the end of a file for switch direction and start deleting in opposite way.
    else if(row == win->data_row - 1)
    {
        //fprintf(fp,"---reached the end of a file--\n");

        //Check whether should we delete inline leftwards
        if(col >0 && (row < win->data_row)){
            //fprintf(fp,"we delete inline leftwards\n");
        
            data = (char*) realloc (data, ((container[row].size) - 1) * sizeof(char*));
            memcpy(data,data,(container[row].size) - 1);
            data[container[row].size - 2] = '\0';
            free(data[container[row].size - 1]);
            
            //Moving the cursor left
            win->current_column --;
            //fprintf(fp,"data:%s size:%d\n",data,strlen(data)+1);
            container[row].data = data;
            container[row].size = strlen(data) + 1; //to include the null terminator
        
        }
        else if(col == 0 && row > 0){
            //fprintf(fp,"go one line up since the current line is empty\n");
        
            free(container[row].data);
            container[row].size = NULL;

            //win->container = realloc(win->container, sizeof(row_container) * (win->data_row -1)); ?????

            //Move the cursor backwards
            win->data_row --;
            win->current_row--;
		        if(win->container[win->current_row].size != 0)
                {
                    win->current_column = win->container[win->current_row].size - 1;
                }

           
        }
        
    }

    //fprintf(fp,"end  : row:%d, column:%d, row_size:%d, data_row:%d, data:%s\n", row, col, container[row].size, win->data_row, container[row].data);
    //fclose(fp);
}

