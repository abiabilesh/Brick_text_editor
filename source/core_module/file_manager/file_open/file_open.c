#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <file_manager.h>

#define TAB_SPACE 8

void container_fill_row(file_container *container, char *line, size_t linelen , int scr_max_column)
{
    int row = container->file_rows;

    /***************************************************************************************
                            The below if loop has to be evaluated
    ****************************************************************************************/
    if(linelen > scr_max_column)
    {
        int off = 0;
        int tmp_len = linelen;
        while(tmp_len > scr_max_column)
        {
            container->lin_con = realloc(container->lin_con, sizeof(line_container) * (row+1));
            container->lin_con[row].data = malloc(scr_max_column);
            
            char *str  = container->lin_con[row].data;
            memcpy(str,(line+off),scr_max_column);
            
            off = off + scr_max_column;
            tmp_len -= scr_max_column;
            container->lin_con[row].size = scr_max_column;
            row++;
        }
        if(tmp_len > 0)
        {
            container->lin_con = realloc(container->lin_con, sizeof(line_container) * (row+1));
            container->lin_con[row].data = (char *) malloc((tmp_len + 1) * sizeof(char*));
            
            char *str  = container->lin_con[row].data;
            memcpy(str,(line+off),tmp_len + 1);
            container->lin_con[row].size = tmp_len + 1;
            row++;
        }
    }
    else
    {
        container->lin_con = realloc(container->lin_con, sizeof(line_container) * (row+1));    
        container->lin_con[row].data = (char*) malloc((linelen + 1) * sizeof(char*));
        
        char *str  = container->lin_con[row].data;
        
         // line_mode_len => 0,1,.......,19  but the linelen is size which is 20 for the same.
        memcpy(str,line,linelen + 1); 
        container->lin_con[row].size = linelen + 1;
        row++;
    }
    
    container->file_rows = row;
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

int editor_file_open(file_container *container, char *file_name, int scr_max_column)
{   
    int ret = 0;
    FILE *fp = fopen(file_name, "r");
    if (!fp){
        //have to add debug api's   
        return -1;
    }

    char *line = NULL;
    size_t linecap = 0;
    ssize_t linelen;
    while ((linelen = getline(&line, &linecap, fp)) != -1) {

        int tab_count;
        tab_count = tab_counter(line, linelen);
        char *line_mod = (char*) malloc((linelen +(tab_count * TAB_SPACE)) * sizeof(char*));

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

        // line_mode_len => 0,1,.......,19  but the linelen is size which is 20 for the same.

        line_mod[line_mod_len] = '\0';
        
        while (line_mod_len > 0 && (line_mod[line_mod_len - 1] == '\n' || line_mod[line_mod_len - 1] == '\r'))
            line_mod_len--;
        
        container_fill_row(container, line_mod, line_mod_len, scr_max_column);
        free(line_mod);

    }
    
    free(line);
    fclose(fp);
    return ret;
}