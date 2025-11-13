// ...existing code...
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "mlx.h"
#include <math.h>
#define M_PI 3.14159265358979323846


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

typedef struct s_screen {
    double width;
    double height;
    double offset_x;
    double offset_y;
    double size;
    double scale;
} t_screen;

typedef struct s_point_map {
    unsigned int width;
    unsigned int height;
    t_point **points;
} t_point_map; 

typedef struct s_vars {
    void *mlx;
    void *win;
    t_screen screen;
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
    
    // Espelha cada linha horizontalmente para corrigir os números
    for (unsigned int r = 0; r < vars->point_map.height; r++)
    {
        for (unsigned int c = 0; c < vars->point_map.width / 2; c++)
        {
            unsigned int opposite = vars->point_map.width - 1 - c;
            
            // Troca os valores z e cores
            int temp_z = vars->point_map.points[r][c].z;
            t_color temp_color = vars->point_map.points[r][c].color;
            
            vars->point_map.points[r][c].z = vars->point_map.points[r][opposite].z;
            vars->point_map.points[r][c].color = vars->point_map.points[r][opposite].color;
            
            vars->point_map.points[r][opposite].z = temp_z;
            vars->point_map.points[r][opposite].color = temp_color;
        }
    }
    
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


t_point project_iso(t_point p, t_vars *vars)
{
    t_point iso;
    double angle = M_PI / 6;  // 30 graus
    
    // Fator de escala para Z (altura) - AUMENTADO para ficar mais vertical
    double z_scale = 1;  // aumenta o efeito da altura

    // Projeção isométrica - p.y - p.x para orientação correta SEM espelhar números
    double iso_x = (p.y - p.x) * cos(angle);
    double iso_y = (p.x + p.y) * sin(angle) - (p.z * z_scale);
    
    // Aplica escala e offset (converte para coordenadas de tela)
    iso.x = (int)(vars->screen.offset_x + iso_x * vars->screen.size);
    iso.y = (int)(vars->screen.offset_y + iso_y * vars->screen.size);
    iso.z = p.z;
    iso.color = p.color;
    
    return (iso);
}

void draw_line(t_point p0, t_point p1, t_vars *vars)
{
    // Calcula diferenças
    int dx = abs(p1.x - p0.x);
    int dy = abs(p1.y - p0.y);
    
    // Determina direção
    int sx = (p0.x < p1.x) ? 1 : -1;
    int sy = (p0.y < p1.y) ? 1 : -1;
    
    int err = dx - dy;
    int x = p0.x;
    int y = p0.y;
    
    // Algoritmo de Bresenham
    while (1)
    {
        // Desenha pixel
        if (x >= 0 && x < vars->screen.width && y >= 0 && y < vars->screen.height)
            mlx_pixel_put(vars->mlx, vars->win, x, y, p0.color.hex);
        
        // Verifica se chegou ao fim
        if (x == p1.x && y == p1.y)
            break;
        
        int e2 = 2 * err;
        
        if (e2 > -dy)
        {
            err -= dy;
            x += sx;
        }
        
        if (e2 < dx)
        {
            err += dx;
            y += sy;
        }
    }
}

int render_next_frame(t_vars *vars)
{
    unsigned int row = 0;
    
    while (row < vars->point_map.height)
    {
        unsigned int col = 0;
        while (col < vars->point_map.width)
        {
            t_point p_atual = vars->point_map.points[row][col];
            t_point iso_atual = project_iso(p_atual, vars);
            
            // Desenha linha para a DIREITA (próxima coluna)
            if (col + 1 < vars->point_map.width)
            {
                t_point p_direita = vars->point_map.points[row][col + 1];
                t_point iso_direita = project_iso(p_direita, vars);
                draw_line(iso_atual, iso_direita, vars);
            }
            
            // Desenha linha para BAIXO (próxima linha)
            if (row + 1 < vars->point_map.height)
            {
                t_point p_abaixo = vars->point_map.points[row + 1][col];
                t_point iso_abaixo = project_iso(p_abaixo, vars);
                draw_line(iso_atual, iso_abaixo, vars);
            }
            
            col++;
        }
        row++;
    }
    
    return (0);
}


int main (int argc, char **argv)
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
    
    // Inicializa MLX primeiro para poder detectar tamanho da tela
    vars.mlx = mlx_init();    
    if (!vars.mlx)
    {
        printf("Erro: mlx_init falhou\n");
        return 1;
    }
    
    // Detecta tamanho do monitor
    int screen_w, screen_h;
    mlx_get_screen_size(vars.mlx, &screen_w, &screen_h);
    
    // Limita ao tamanho de uma tela comum (caso detecte múltiplas telas)
    if (screen_w > 2560) screen_w = 2560;  // limita a 2K
    if (screen_h > 1440) screen_h = 1440;
    
    // Usa 80% do tamanho da tela (mais conservador)
    vars.screen.width = screen_w * 0.8;
    vars.screen.height = screen_h * 0.8;
    
    printf("Monitor detectado: %dx%d | Janela: %.0fx%.0f\n", 
           screen_w, screen_h, vars.screen.width, vars.screen.height);
    
    // Calcula escala baseada no tamanho da janela e do mapa
    double usable_width = vars.screen.width * 0.85;  // usa 85% da largura
    double usable_height = vars.screen.height * 0.85; // usa 85% da altura
    
    // Calcula quantos pixels cada ponto deve ocupar (considerando projeção isométrica)
    // A projeção isométrica expande a largura por cos(30°) ≈ 0.866
    double projected_width = vars.point_map.width * 0.866 + vars.point_map.height * 0.866;
    double projected_height = vars.point_map.width * 0.5 + vars.point_map.height * 0.5;
    
    double scale_x = usable_width / projected_width;
    double scale_y = usable_height / projected_height;
    
    // Usa a menor escala para garantir que cabe na tela
    vars.screen.size = (scale_x < scale_y) ? scale_x : scale_y;
    
    if (vars.screen.size < 1)
        vars.screen.size = 1;
    
    printf("Mapa: %dx%d | Escala: %.2f pixels/ponto\n",
           vars.point_map.width, vars.point_map.height, vars.screen.size);
    
    // Centraliza o mapa
    vars.screen.offset_x = vars.screen.width / 2;
    vars.screen.offset_y = vars.screen.height / 2.5;
    
    vars.win = mlx_new_window(vars.mlx, vars.screen.width, vars.screen.height, "FDF - Wireframe 3D");
    if (!vars.win)
    {
        printf("Erro: mlx_new_window falhou\n");
        return 1;
    }

    mlx_loop_hook(vars.mlx, render_next_frame, &vars);
    mlx_loop(vars.mlx);

    free(content);
    close(fd);
    return 0;
}