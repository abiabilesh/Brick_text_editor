#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <Brick.h>
#include <Brick_core.h>
#include <Brick_exit.h>

#define CTRL_KEY(k) (k & 0x1f)

struct brick_win_size brick_core_win;

enum editorkey{
	ARROW_UP = 1000,
	ARROW_DOWN,
	ARROW_LEFT,
	ARROW_RIGHT
};

int brick_read_key(void)
{
	int ret;
	char c = '\0';
	while(ret = read(STDIN_FILENO,&c,1) != 1){
		if((ret== -1) && (errno != EAGAIN)){
		die("main:read");
	  }
	}
	
	if(c == '\x1b'){
		char str[3];
		if(read(STDIN_FILENO,&str[0],1) != 1)
			return '\x1b';
		if(read(STDIN_FILENO,&str[1],1) != 1)
			return '\x1b';
		if(str[0] == '['){
			switch(str[1]){
				case 'A': return ARROW_UP;
				case 'B': return ARROW_DOWN;
				case 'C': return ARROW_RIGHT;
				case 'D': return ARROW_LEFT;
			}
		}
		return '\x1b';	
	}
	else
		return c;
}


void brick_core_move(int arrow_movement)
{
	switch(arrow_movement){
		case ARROW_UP:
			if(brick_core_win.current_row != 0)
				brick_core_win.current_row--;
			break;
		case ARROW_DOWN:
			if(brick_core_win.current_row != brick_core_win.row - 1)
				brick_core_win.current_row++;
			break;
		case ARROW_LEFT:
			if(brick_core_win.current_column != 0)
				brick_core_win.current_column--;
			break;
		case ARROW_RIGHT:
			if(brick_core_win.current_column != brick_core_win.col -1)
				brick_core_win.current_column++;
			break;
	}	
		
}


void brick_core_inloop(void)
{
	int c;
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
	}
}

void brick_refresh_screen(void)
{
	Brick_buffer bufs = BUF_INIT;
	buffer_append(&bufs, "\x1b[?25l", 6);	//hide the cursor
	buffer_append(&bufs, "\x1b[H", 3);		//move the cursor to start
	brick_draw_rows(&bufs);
	buffer_append(&bufs, "\x1b[H", 3);		//move the cursor to start

	char move[32];
	snprintf(&move,sizeof(move),"\x1b[%d;%dH",brick_core_win.current_row+1,brick_core_win.current_column+1);
	buffer_append(&bufs,move,strlen(move));
	
	buffer_append(&bufs, "\x1b[?25h", 6);	//show the cursor		
	write(STDOUT_FILENO, bufs.buf, bufs.length);
	
	buffer_free(&bufs);
}


void brick_draw_rows(Brick_buffer *bufs) 
{
  int y;
  for (y = 0; y < brick_core_win.col; y++) {
    buffer_append(bufs, "\x1b[K", 3);		//clean the current line
	buffer_append(bufs, "\r\n~", 3);
  }
}

void brick_core_init(Brick brick)
{
	brick_core_win.row = brick.brick_row;
	brick_core_win.col = brick.brick_column;
	brick_core_win.current_row = 0;
	brick_core_win.current_column = 0;
}