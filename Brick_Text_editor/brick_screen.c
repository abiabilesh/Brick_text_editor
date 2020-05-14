#include <stdio.h> 
#include <stdlib.h> 
#include <pthread.h> 
#include <stdint.h>
#include <brick_core.h>
#include "brick_screen.h"

//extern uint8_t screen_loop_flag;

#define REFRESH_120FPS
 
void *myThreadFun(void *vargp) 
{ 
    while(1){
        brick_refresh_screen();

#ifdef REFRESH_60FPS
        usleep(16000); //refreshing @60fps
#elif defined(REFRESH_120FPS)
        usleep(8000);  //refreshing @120fps
#elif defined(REFRESH_360FPS)
	usleep(2000);  //refreshing @360fps
#else 
        usleep(33000); //refreshing @30fps
#endif    

    }

    return NULL; 
} 
   
int brick_screen_init(void) 
{ 
    pthread_t thread_id; 
   
    brick_refresh_screen(); //update the screen once and let screen_thread \
                             take over from next time    
    
    pthread_create(&thread_id, NULL, myThreadFun, NULL); 
    return thread_id; 
}

