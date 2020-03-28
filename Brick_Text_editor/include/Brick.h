
typedef struct Brick
{
 struct termios org_term;
 int brick_row;
 int brick_column;
}Brick;

Brick brick; 

void terminos_raw_clear(void);
void terminos_raw(void);
void get_terminos_property(void);