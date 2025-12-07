#include "../fdf.h"
#include <string.h>
#include <stdlib.h>

int try_parse(t_vars *vars, char *map)
{
    if(!vars || !map)
        return (0);

    unsigned int rows = count_char(map, '\n');
    if (rows == 0)
        return (0);
    
    vars->point_map.points = malloc((rows + 1) * sizeof(t_point *));
    if (!vars->point_map.points)
        return (0);
    
    vars->point_map.height = 0;
    vars->point_map.width = 0;
    
    unsigned int row = 0;
    char *line_ref;
    char *line = strtok_r(map, "\n", &line_ref);
    
    while(line != NULL)
    {
        char *temp_line = strdup(line);
        if (!temp_line)
        {
            free_points(vars);
            return (0);
        }
        
        unsigned int cols = count_tokens(temp_line);
        free(temp_line);
        
        if (cols == 0)  // linha vazia, pula
        {
            line = strtok_r(NULL, "\n", &line_ref);
            continue;
        }
        
        if (vars->point_map.width == 0)
            vars->point_map.width = cols;
        else if (vars->point_map.width != cols)
        {
            free_points(vars);
            return (0);
        }
        
        vars->point_map.points[row] = malloc(cols * sizeof(t_point));
        if (!vars->point_map.points[row])
        {
            free_points(vars);
            return (0);
        }
        
        unsigned int col = 0;
        char *token_ref;
        char *token = strtok_r(line, " ", &token_ref);
        
        while (token != NULL && col < cols)
        {
            char *color_part = strchr(token, ',');
            int z = atoi(token);
            
            t_color color;
            if (color_part)
            {
                color.hex = (unsigned int)strtol(color_part + 1, NULL, 16);
            }
            else
            {
                color.hex = 0xFFFFFF;
            }
            
            vars->point_map.points[row][col].x = col;
            vars->point_map.points[row][col].y = row;
            vars->point_map.points[row][col].z = z;
            vars->point_map.points[row][col].color = color;
            
            token = strtok_r(NULL, " ", &token_ref);
            col++;
        }
        
        line = strtok_r(NULL, "\n", &line_ref);
        row++;
    }
    
    vars->point_map.points[row] = NULL;
    vars->point_map.height = row;
    
    return (1);
}