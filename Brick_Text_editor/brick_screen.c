#include <stdio.h> 
#include <stdlib.h> 
#include <pthread.h> 
#include <stdint.h>
#include <brick_core.h>
#include "brick_screen.h"

extern uint8_t screen_loop_flag;
  
void *myThreadFun(void *vargp) 
{ 
    while(1){
        if(screen_loop_flag == 1){ 
            brick_refresh_screen();
        screen_loop_flag = 0;   
        }
    }

    return NULL; 
} 
   
int brick_screen_init(void) 
{ 
    pthread_t thread_id; 
    pthread_create(&thread_id, NULL, myThreadFun, NULL); 
    return thread_id; 
}

