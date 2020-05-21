#ifndef brick_core
#define brick_core

#define BRICK_VERSION "0.1"
#include <brick_init.h>

typedef struct row_container
{
   int size;
   char *data;
}row_container;


typedef struct message_bar
{
   char message[80];
   time_t message_time;
}message_bar;

typedef struct brick_win_size
{
   int row;
   int col;
   int current_row;
   int current_column;
   int data_row;
   int row_off;
   int col_off;
   row_container *container;
   char *filename;
   message_bar *msg_bar;  
   
}brick_window;

void brick_core_init(Brick brick, char *filename);
int brick_read_key(void);
void brick_core_inloop(void);
void brick_clear_screen(void);

#endif
