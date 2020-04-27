#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <brick_buf_api.h>
#include <brick_core.h>
#include <brick_file_init.h>
#include <brick_exit.h>


#include <stdint.h>
uint8_t screen_loop_flag = 0;

#define CTRL_KEY(k) (k & 0x1f)

struct brick_win_size win;

enum editorkey{
    ARROW_UP = 1000,
    ARROW_DOWN,
    ARROW_LEFT,
    ARROW_RIGHT,
    ARROW_PG_UP,
    ARROW_PG_DOWN,
    ARROW_HOME
};

int brick_read_key(void)
{
    int ret;
    char c = '\0';
    while(ret = read(STDIN_FILENO,&c,1) != 1)
    {
        if((ret== -1) && (errno != EAGAIN))
        {
            die("main:read");
        }
    }
    
    if(c == '\x1b')
    {
        char str[3];
        if(read(STDIN_FILENO,&str[0],1) != 1)
            return '\x1b';
        if(read(STDIN_FILENO,&str[1],1) != 1)
            return '\x1b';
        if(str[0] == '[')
        {
            if(str[1] >= '1' && str[1] <= '9')
            {
                if(read(STDIN_FILENO,&str[2],1) != 1)
                    return '\x1b';
                if(str[2] == '~')
                {
                    switch(str[1])
                    {
                        case '1':
                        case '7':return ARROW_HOME;
                        case '5':return ARROW_PG_UP;
                        case '6':return ARROW_PG_DOWN;
                    }
                }
                else 
                    return '\x1b';
            }
            else
            {
                switch(str[1]){
                    case 'A': return ARROW_UP;
                    case 'B': return ARROW_DOWN;
                    case 'C': return ARROW_RIGHT;
                    case 'D': return ARROW_LEFT;
                    case 'H': return ARROW_HOME;
                }
            }
        }
        return '\x1b';    
    }
    else
    {
        return c;
    }
}


void brick_core_move(int arrow_movement)
{
    switch(arrow_movement)
    {
        case ARROW_UP:
            if(win.current_row != 0)
                win.current_row--;
            break;
            
        case ARROW_DOWN:
            if(win.current_row != win.row - 1)
                win.current_row++;
            break;
            
        case ARROW_LEFT:
            if(win.current_column != 0)
                win.current_column--;
            break;
            
        case ARROW_RIGHT:
            if(win.current_column != win.col -1)
                win.current_column++;
            break;
    }    
        
}


void brick_core_inloop(void)
{
    int c, tmp_rows;
    tmp_rows = win.row;
    while(screen_loop_flag == 1);
        c = brick_read_key();
    switch(c)
    {
        case CTRL_KEY('q'):
            write(STDOUT_FILENO, "\x1b[2J", 4);
            write(STDOUT_FILENO, "\x1b[H", 3);
            exit(0);
            break;
            
        case ARROW_UP:
        case ARROW_DOWN:
        case ARROW_LEFT:
        case ARROW_RIGHT:
            brick_core_move(c);
            break;
            
        case ARROW_HOME:
            win.current_row = 0;
            win.current_column = 0;
            break;
        
        case ARROW_PG_UP:
        case ARROW_PG_DOWN:
            while(tmp_rows)
            {
                tmp_rows--;
                (c == ARROW_PG_UP) ? brick_core_move(ARROW_UP) : brick_core_move(ARROW_DOWN);
            }
            break;    
    }
    screen_loop_flag = 1; //brick screen thread will clear this bit 
}

void brick_refresh_screen(void)
{
    Brick_buffer bufs = BUF_INIT;
    buffer_append(&bufs, "\x1b[?25l", 6);     //hide the cursor
    buffer_append(&bufs, "\x1b[H", 3);        //move the cursor to start
    brick_draw_rows(&bufs);
    buffer_append(&bufs, "\x1b[H", 3);        //move the cursor to start

    char move[32];
    snprintf(&move,sizeof(move),"\x1b[%d;%dH", (win.current_row + 1), (win.current_column + 1));
    buffer_append(&bufs,move,strlen(move));

    buffer_append(&bufs, "\x1b[?25h", 6);     //show the cursor        
    write(STDOUT_FILENO, bufs.buf, bufs.length);

    buffer_free(&bufs);
}


void brick_draw_rows(Brick_buffer *bufs) 
{
    
    for (int y = 0; y < win.row; y++) 
    {
        buffer_append(bufs,"\x1b[K", 3); //clean the current line
        if(y >= win.data_row)
        {
            buffer_append(bufs,"~", 1);
        }
        else
        {
            buffer_append(bufs,win.container[y].data,win.container[y].size);
        }
        
        if(y < win.row-1)
        buffer_append(bufs,"\r\n",2);
    }

} 

void brick_core_init(Brick brick)
{
    win.row = brick.brick_row;
    win.col = brick.brick_column;
    win.current_row = 0;
    win.current_column = 0;
    win.container = NULL;
    win.data_row = 0;
    brick_open_file(&win,"./hello.txt");
}
