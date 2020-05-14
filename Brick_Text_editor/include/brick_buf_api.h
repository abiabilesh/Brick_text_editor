#define BUF_INIT {NULL,0};

typedef struct Brick_buffer
{
	char *buf;
	int length;
}Brick_buffer;


int buffer_append(Brick_buffer *brick, char *buf, int length);
int buffer_free(Brick_buffer *buffer);