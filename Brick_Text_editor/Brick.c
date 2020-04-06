#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>
#include <Brick.h>
#include <Brick_core.h>
#include <Brick_sys.h>
#include <Brick_exit.h>

Brick brick;

int buffer_append(Brick_buffer *brick, char *buf, int length)
{
	char *new = realloc(brick->buf, brick->length+length);
	if(new == NULL)
		return -1;
	memcpy(&new[brick->length],buf,length);
	brick->buf = new;
	brick->length += length;
}

int buffer_free(Brick_buffer *buffer)
{
	free(buffer->buf);
}

void terminos_raw_clear(void)
{
	if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &brick.org_term) == -1)
		die("terminos_raw_clear:tcsetattr");
}

void terminos_raw(void)
{
	struct termios term;
	
	if(tcgetattr(STDIN_FILENO, &brick.org_term) == -1)
		die("terminos_raw:tcgetattr");
	atexit(terminos_raw_clear);
	
	term = brick.org_term;
	term.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
	term.c_oflag &= ~(OPOST);
	term.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	term.c_cflag |= (CS8);
	term.c_cc[VMIN] = 0;
    term.c_cc[VTIME] = 1;
	
	if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &term) == -1)
		die("terminos_raw:tcsetattr");
}

void get_terminos_property(void)
{
	if(brick_get_window_size(&brick.brick_row, &brick.brick_column) == -1){
		die("get_terminos_property:brick_get_window_size");
	}
}

int main()
{
	terminos_raw();
	get_terminos_property();
	brick_core_init(brick);
	while(1)
	{
		brick_refresh_screen();
		brick_core_inloop();
	}
	return 0;
}