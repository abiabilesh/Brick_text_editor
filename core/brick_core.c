#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <brick_buf_api.h>
#include <brick_core.h>
#include <brick_file_init.h>
#include <brick_exit.h>
#include <stdint.h>

//uint8_t screen_loop_flag = 0;

#define CTRL_KEY(k) (k & 0x1f)

struct brick_win_size win;

enum editorkey{
    ARROW_UP = 1000,
    ARROW_DOWN,
    ARROW_LEFT,
    ARROW_RIGHT,
    ARROW_PG_UP,
    ARROW_PG_DOWN,
    ARROW_HOME,
    ARROW_END
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
		    	case '4':return ARROW_END;
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
    row_container *row = (win.current_row >= win.data_row) ? NULL : &win.container[win.current_row];

    switch(arrow_movement)
    {
        case ARROW_UP:
            if(win.current_row != 0)
                win.current_row--;
            break;
            
        case ARROW_DOWN:
	    if(win.current_row < win.data_row)
		win.current_row++;
            break;
            
        case ARROW_LEFT:
            if(win.current_column != 0)
                win.current_column--;
	    else if(win.current_row != 0){
	    	win.current_row--;
		win.current_column = win.container[win.current_row].size;
            }
            break;
            
        case ARROW_RIGHT:
            if(row && win.current_column < row->size -1)
                win.current_column++;
 	    else if(win.current_column == row->size -1 || win.current_column == row->size){
		win.current_row++;
		win.current_column = 0;
	    }
            break;
    }    
    
    row = (win.current_row >= win.data_row) ? NULL : &win.container[win.current_row];
    int rowsize = row ? row->size : 0;
    if(win.current_column >  rowsize){
	win.current_column = rowsize ;
    }
}


void brick_core_inloop(void)
{
    int c, tmp_rows;
    tmp_rows = win.row;
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
            win.current_column = 0;
            break;
       
	case ARROW_END:
	    if(win.current_column < win.container[win.current_row].size -1)
	    win.current_column = win.container[win.current_row].size - 1;
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
    //screen_loop_flag = 1; //brick screen thread will clear this bit 
}

void editor_scroll_row(void)
{
    if(win.current_row >= win.row_off + win.row){
	win.row_off = win.current_row - win.row + 1 ;
    }

    if(win.current_row < win.row_off)
	win.row_off = win.current_row;
 
    if(win.current_column >= win.col_off + win.col){
	win.col_off = win.current_column - win.col + 1 ;
    }
  
    if(win.current_column < win.col_off)
	win.col_off = win.current_column;
}

void brick_message_bar(Brick_buffer *bufs)
{
    char *message = win.msg_bar[0].message;
    time_t msg_time = win.msg_bar[0].message_time;
    
    buffer_append(bufs,"\x1b[K", 3); //clean the current line
    
    int len = strlen(message);
    if(len > win.col)
        len = win.col;
    
    if(len && (time(NULL) - msg_time) < 2)
        buffer_append(bufs, message, len); 
}

void brick_status_bar(Brick_buffer *bufs)
{
    buffer_append(bufs, "\x1b[7m", 4);
    char status[80], coordinates[80];
    int len = snprintf(status,sizeof(status), "%.40s - %d lines", \
    win.filename ? win.filename : "[No Name]", win.data_row - 1);
    if(len > win.col) 
        len = win.col;
    buffer_append(bufs, status, len);
    
    int clen = snprintf(coordinates, sizeof(coordinates), \
    "%d,%d       %d%%", win.current_row,win.current_column, \
    ((win.current_row+1)*100)/win.data_row);

    while(len < win.col){
        if(win.col - len == clen){
	    buffer_append(bufs, coordinates, clen);
            break;
        }
        else{
            buffer_append(bufs, " ", 1);
	    len ++;
	}
    }
    buffer_append(bufs, "\x1b[m", 3);
    buffer_append(bufs,"\r\n",2);
}


void brick_refresh_screen(void)
{
    editor_scroll_row();
    Brick_buffer bufs = BUF_INIT;
    buffer_append(&bufs, "\x1b[?25l", 6);     //hide the cursor
    buffer_append(&bufs, "\x1b[H", 3);        //move the cursor to start
    brick_draw_rows(&bufs);
        
    brick_status_bar(&bufs);
    brick_message_bar(&bufs);
    
    buffer_append(&bufs, "\x1b[H", 3);        //move the cursor to start

    char move[32];
    snprintf(&move,sizeof(move),"\x1b[%d;%dH", (win.current_row - win.row_off + 1), (win.current_column - win.col_off + 1));
    buffer_append(&bufs,move,strlen(move));

    buffer_append(&bufs, "\x1b[?25h", 6);     //show the cursor        
    write(STDOUT_FILENO, bufs.buf, bufs.length);

    buffer_free(&bufs);
}


void brick_draw_rows(Brick_buffer *bufs) 
{
    
    for (int y = 0; y < win.row; y++) 
    {
        int filerow = y + win.row_off;
        buffer_append(bufs,"\x1b[K", 3); //clean the current line
        if(filerow >= win.data_row){
	    if(win.data_row == 0 && y == win.row/3){
	        char welcome[80];
		int welcomelen = snprintf(welcome, sizeof(welcome),
                "BRICK_TEXT_EDITOR -- version %s", BRICK_VERSION);
                if (welcomelen > win.col) welcomelen = win.col;
                int padding = (win.col - welcomelen) / 2;
                if (padding) {
                    buffer_append(bufs, "~", 1);
                    padding--;
                }
                while (padding--) buffer_append(bufs, " ", 1);
                buffer_append(bufs, welcome, welcomelen);
                
            }else {
                buffer_append(bufs, "~", 1);
            }
        }
        else
        {
            int len = win.container[filerow].size - win.col_off;
	    if(len < 0)
                len = 0;
	    if(win.container[filerow].size > win.col) len = win.col;
                buffer_append(bufs,&win.container[filerow].data[win.col_off],len);
        }
       
        buffer_append(bufs,"\x1b[K",3); 
        buffer_append(bufs,"\r\n",2);
    }

} 

void core_msgbar_init(Brick brick)
{
    win.msg_bar = (message_bar*) malloc(sizeof(message_bar) * 1);;
     
    message_bar *msg_bar = win.msg_bar;
    char *brick_msg = brick.message_bar;
    
    msg_bar[0].message[0] = '\0';
    msg_bar[0].message_time = brick.message_time;

    char *win_msg = msg_bar[0].message;
    strcpy(win_msg, brick_msg);
}

void brick_core_init(Brick brick, char *filename)
{
    win.row = brick.brick_row;
    win.row -= 2; //status bar and message bar 
    win.col = brick.brick_column;
    win.current_row = 0;
    win.current_column = 0;
    win.container = NULL;
    win.data_row = 0;
    win.row_off = 0;
    win.col_off = 0;
    win.filename = NULL;
    
    core_msgbar_init(brick);
    brick_open_file(&win,filename);
}
