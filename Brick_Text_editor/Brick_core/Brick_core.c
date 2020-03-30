#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <Brick.h>
#include <Brick_core.h>
#include <Brick_exit.h>

#define CTRL_KEY(k) (k & 0x1f)

char brick_read_key(void)
{
	int ret;
	char c = '\0';
	while(ret=read(STDIN_FILENO,&c,1)){
		if((ret== -1) && (errno != EAGAIN)){
		die("main:read");
	  }
	}
	return c;
}

void brick_core_inloop(void)
{
	char c = '\0';
	c = brick_read_key();
	
	switch(c)
	{
		case CTRL_KEY('q'):
			exit(0);
			break;
	}
	
	if(iscntrl(c)){
		printf("%d\r\n",c);
	}else{
		printf("%d('%c')\r\n",c,c);
	}

}

void brick_refresh_screen(void)
{
	Brick_buffer bufs = BUF_INIT;
	buffer_append(&bufs, "\x1b[?25l", 6);
	buffer_append(&bufs,"\x1b[2J", 4);
	buffer_append(&bufs, "\x1b[H", 3);
	brick_draw_rows(&bufs);
	buffer_append(&bufs, "\x1b[H", 3);
	buffer_append(&bufs, "\x1b[?25l", 6);
	write(STDOUT_FILENO, bufs.buf, bufs.length);
	buffer_free(&bufs);
}

void brick_clean_screen(void)
{
	Brick_buffer bufs = BUF_INIT;
	buffer_append(&bufs, "\x1b[?25l", 6);
	buffer_append(&bufs,"\x1b[2J", 4);
	buffer_append(&bufs, "\x1b[H", 3);
	buffer_append(&bufs, "\x1b[H", 3);
	buffer_append(&bufs, "\x1b[?25l", 6);
	write(STDOUT_FILENO, bufs.buf, bufs.length);
	buffer_free(&bufs);
}

void brick_draw_rows(Brick_buffer *bufs) 
{
  int y;
  for (y = 0; y < brick_core_win.col; y++) {
    buffer_append(bufs, "\r\n~", 3);
	buffer_append(bufs, "\x1b[K", 3);
  }
}

void brick_core_init(Brick brick)
{
	brick_core_win.row = brick.brick_row;
	brick_core_win.col = brick.brick_column;
}