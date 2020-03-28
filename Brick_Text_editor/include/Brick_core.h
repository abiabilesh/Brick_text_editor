
struct brick_win_size
{
 int row;
 int col; 
};

struct brick_win_size brick_core_win;

void brick_core_init(Brick brick);
char brick_read_key(void);
void brick_core_inloop(void);
void brick_clear_screen(void);
void brick_draw_rows(void);