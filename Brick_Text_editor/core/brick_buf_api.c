#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <brick_buf_api.h>

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
