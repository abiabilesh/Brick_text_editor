
typedef struct cursor_coordinates cursor_coordinates;
typedef struct screen_max_coordinates screen_max_coordinates;
typedef struct screen_manager screen_manager; 


struct cursor_coordinates
{
    int current_column;     //integer pointing the current column
    int current_row;        //integer pointing the current row
};

struct screen_max_coordinates
{
    int scr_max_column;     //integer pointing the maximum screen column
    int scr_max_row;        //integer pointing the maximum screen row
};

struct screen_manager
{
    cursor_coordinates *cur_cord;           //cursor coordinate structure for maintaining
    screen_max_coordinates *win_cord;       //window coordinate structure for maintaining
};
