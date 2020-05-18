#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <brick_file_init.h>

#define TAB_SPACE 8

void container_fill_row(struct brick_win_size *win,char *line, size_t linelen)
{
    int row = win->data_row;
    
    if(linelen > win->col)
    {
        int off = 0;
        int tmp_len = linelen;
        while(tmp_len > win->col)
        {
            win->container = realloc(win->container, sizeof(row_container) * (row+1));
            win->container[row].data = malloc(win->col);
            
            char *str  = win->container[row].data;
            memcpy(str,(line+off),win->col);
            
            off = off + win->col;
            tmp_len -= win->col;
            win->container[row].size = win->col;
            row++;
        }
        if(tmp_len > 0)
        {
            win->container = realloc(win->container, sizeof(row_container) * (row+1));
            win->container[row].data = malloc(tmp_len);
            
            char *str  = win->container[row].data;
            memcpy(str,(line+off),tmp_len);
            win->container[row].size = tmp_len;
            row++;
        }
    }
    else
    {
        win->container = realloc(win->container, sizeof(row_container) * (row+1));    
        win->container[row].data = malloc(linelen);
        
        char *str  = win->container[row].data;
        memcpy(str,line,linelen);
      
        win->container[row].size = linelen;
        row++;
    }
    
    win->data_row = row;
}


int tab_counter(char *line, size_t linecap) 
{
    int off = linecap - 1;
        int tab_cnt = 0;
    while(off){
                if(line[off] == '\t')
                tab_cnt++; 
        off--;
    }
    return tab_cnt;
}

void brick_open_file(struct brick_win_size *win,char *filename)
{
    free(win->filename);
    win->filename = strdup(filename);

    FILE *fp = fopen(filename, "r");
    if (!fp) 
        die("fopen");
    char *line = NULL;
    size_t linecap = 0;
    ssize_t linelen;
    while ((linelen = getline(&line, &linecap, fp)) != -1) {
        
        int tab_count;
        tab_count = tab_counter(line, linecap);
        char *line_mod = malloc((linecap+(tab_count*TAB_SPACE)) * sizeof(char*) + 1);
        int line_mod_len = 0, line_org_len = 0;
  
        while(line_org_len < linelen - 1){
            if(line[line_org_len] == '\t'){
                int tmp = line_mod_len + TAB_SPACE;
                while(line_mod_len < tmp){
                    line_mod[line_mod_len] = ' ';
                    line_mod_len++;
                }
            }
            else{
                line_mod[line_mod_len] = line[line_org_len];
                line_mod_len++;
            }
            line_org_len++;                
        }                
         
        line_mod[line_mod_len] = '\0';
        while (line_mod_len > 0 && (line_mod[line_mod_len - 1] == '\n' || line_mod[line_mod_len - 1] == '\r'))
            line_mod_len--;

        container_fill_row(win, line_mod, line_mod_len);
        free(line_mod);
    }
      
    free(line);
    fclose(fp);
}
