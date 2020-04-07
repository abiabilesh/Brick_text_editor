
#define BUF_INIT {NULL,0};

typedef struct Brick
{
	struct termios org_term;
	int brick_row;
	int brick_column;
}Brick;

typedef struct Brick_buffer
{
	char *buf;
	int length;
}Brick_buffer;

void terminos_raw_clear(void);
void terminos_raw(void);
void get_terminos_property(void);
int buffer_append(Brick_buffer *brick, char *buf, int length);