#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#define BUFFER_SIZE 1024

typedef union u_color{
    unsigned int hex;
    struct { unsigned char r, g, b, a; };
}   t_color;

typedef struct s_point {
    int x;
    int y;
    int z;
    t_color color;
} t_point;

typedef struct s_point_map {
    unsigned int width;
    unsigned int height;
    t_point **points;
} t_point_map; 

typedef struct s_vars {
    t_point_map point_map;
} t_vars;

int count_char(const char *str, char c) {
    int count = 0;
    while (*str) {
        if (*str == c)
            count++;
        str++;
    }
    return count;
}

int free_points(t_vars *vars)
{
    if(!vars || !vars->point_map.points)
        return (0);
    unsigned int x = 0;
    while(vars->point_map.points[x])
    {
        free(vars->point_map.points[x]);
        x++;
    }
    free(vars->point_map.points);
    return (1);
}

int try_parse(t_vars *vars, char *map)
{
    if(!vars || !map)
        return (0);
    
    // Conta linhas não-vazias
    unsigned int rows = count_char(map, '\n');
    if (rows == 0)
        return (0);
    
    // Aloca array de ponteiros para linhas
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
        // Primeiro passa: conta quantos tokens reais existem
        char *temp_line = strdup(line);
        if (!temp_line)
        {
            free_points(vars);
            return (0);
        }
        
        unsigned int cols = 0;
        char *temp_ref;
        char *temp_item = strtok_r(temp_line, " \t", &temp_ref);
        while (temp_item != NULL)
        {
            cols++;
            temp_item = strtok_r(NULL, " \t", &temp_ref);
        }
        free(temp_line);
        
        if (cols == 0)
        {
            line = strtok_r(NULL, "\n", &line_ref);
            continue;
        }
        
        // Define width baseado na primeira linha
        if (row == 0)
            vars->point_map.width = cols;
        
        // Aloca array de pontos para esta linha
        vars->point_map.points[row] = malloc(cols * sizeof(t_point));
        if (!vars->point_map.points[row])
        {
            free_points(vars);
            return (0);
        }
        
        // Parse cada valor na linha
        unsigned int col = 0;
        char *item_ref;
        char *item = strtok_r(line, " \t", &item_ref);
        
        while (item != NULL && col < cols)
        {
            // Parse cor se existir (formato: valor,0xRRGGBB)
            char *color_str = strchr(item, ',');
            if(color_str)
                vars->point_map.points[row][col].color.hex = strtoul(color_str + 1, NULL, 0);
            else
                vars->point_map.points[row][col].color.hex = 0xFFFFFF;
            
            // Armazena coordenadas
            vars->point_map.points[row][col].x = col;  // x = coluna (horizontal)
            vars->point_map.points[row][col].y = row;  // y = linha (vertical)
            vars->point_map.points[row][col].z = atoi(item);
            
            col++;
            item = strtok_r(NULL, " \t", &item_ref);
        }
        
        line = strtok_r(NULL, "\n", &line_ref);
        row++;
    }
    
    vars->point_map.points[row] = NULL;
    vars->point_map.height = row;
    
    return (1);
}

char *read_file_to_string(int fd)
{
    char buffer[BUFFER_SIZE];
    char *content = NULL;
    size_t total = 0;
    ssize_t bytesRead;

    while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) {
        char *tmp = realloc(content, total + (size_t)bytesRead + 1);
        if (!tmp) {
            free(content);
            return NULL;
        }
        content = tmp;
        memcpy(content + total, buffer, (size_t)bytesRead);
        total += (size_t)bytesRead;
        content[total] = '\0';
    }

    if (bytesRead < 0) {
        free(content);
        return NULL;
    }

    return content;
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "usage: %s <file>\n", argv[0]);
        return 1;
    }
    
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        perror("open");
        return 1;
    }
    
    char *content = read_file_to_string(fd);
    if (!content) {
        perror("read_file_to_string");
        return 1;
    }
    
    t_vars vars;
    if(!try_parse(&vars, content))
    {
        printf("Erro: falha ao analisar o mapa\n");
        free(content);
        close(fd);
        return (1);
    }
    
    printf("Mapa: %d linhas x %d colunas\n", vars.point_map.height, vars.point_map.width);
    printf("\nPrimeiras 3 linhas parseadas:\n");
    for (unsigned int i = 0; i < 3 && i < vars.point_map.height; i++) {
        printf("Linha %d: ", i);
        for (unsigned int j = 0; j < vars.point_map.width; j++) {
            printf("%d ", vars.point_map.points[i][j].z);
        }
        printf("\n");
    }
    
    free(content);
    close(fd);
    free_points(&vars);
    return 0;
}
