
struct brick_win_size
{
 int row;
 int col;
 int current_row;
 int current_column;
};


void brick_core_init(Brick brick);
int brick_read_key(void);
void brick_core_inloop(void);
void brick_clear_screen(void);
void brick_draw_rows(Brick_buffer *buf);
