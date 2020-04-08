#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <brick_exit.h>

int brick_ioctl_fallback(int *rows, int *cols)
{
	unsigned int i = 0;
	char buffer[32];
	
	if(write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12)!=12)	
		die("brick_ioctl_fallback");
	
	if(write(STDOUT_FILENO, "\x1b[6n",4)!=4)
		die("brick_ioctl_fallback");
	
	while(i < sizeof(buffer) - 1)
	{  
		if(read(STDIN_FILENO,&buffer[i],1) != 1 )
			break;
		if(buffer[i] == 'R')
			break;
		i++;
	}
	
	buffer[i]='\0';
	if(buffer[0]!='\x1b' || buffer[1] != '[')
		return -1;
	if(sscanf(&buffer[2], "%d;%d", rows, cols) != 2)
		return -1;

	return 0;
	
}

int brick_get_window_size(int *rows, int *cols)
{
	int ret = 0;
	struct winsize ws;
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) 
	{
		ret = brick_ioctl_fallback(rows,cols);
		if(ret)
			{
				die("brick_get_window_size:brick_ioctl_fallback");
				return -1;
			}
		return 0;
	} 
	else 
	{
		*cols = ws.ws_col;
		*rows = ws.ws_row;
		return 0;
	}
}