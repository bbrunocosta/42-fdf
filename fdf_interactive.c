#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "mlx.h"

#define BUFFER_SIZE 1024
#define M_PI 3.14159265358979323846

// Keycodes para Linux
#define KEY_ESC 65307
#define KEY_LEFT 65361
#define KEY_RIGHT 65363
#define KEY_UP 65362
#define KEY_DOWN 65364
#define KEY_W 119
#define KEY_S 115
#define KEY_A 97
#define KEY_D 100
#define KEY_Q 113
#define KEY_E 101
#define KEY_PLUS 61      // tecla =
#define KEY_MINUS 45     // tecla -

typedef union u_color {
    unsigned int hex;
    struct { unsigned char r, g, b, a; };
} t_color;

typedef struct s_point {
    double x;
    double y;
    double z;
    t_color color;
} t_point;

// Matriz 3x3 para rotações acumuladas
typedef struct s_matrix {
    double m[3][3];
} t_matrix;

typedef struct s_rotation {
    double x;  // rotação em torno do eixo X
    double y;  // rotação em torno do eixo Y
    double z;  // rotação em torno do eixo Z
} t_rotation;

typedef struct s_translation {
    double x;  // translação no eixo X
    double y;  // translação no eixo Y
} t_translation;

typedef struct s_screen {
    double width;
    double height;
    double offset_x;
    double offset_y;
    double size;
    void *img;
    char *addr;
    int bits_per_pixel;
    int line_length;
    int endian;
} t_screen;

typedef struct s_point_map {
    unsigned int width;
    unsigned int height;
    t_point **points;
} t_point_map;

typedef struct s_keys {
    int w;
    int s;
    int a;
    int d;
    int q;
    int e;
    int up;
    int down;
    int left;
    int right;
    int plus;
    int minus;
} t_keys;

typedef struct s_vars {
    void *mlx;
    void *win;
    t_screen screen;
    t_point_map point_map;
    t_rotation rotation;
    t_translation translation;
    t_keys keys;
    double zoom;
    t_matrix rot_matrix;  // Matriz de rotação acumulada
} t_vars;

int count_char(const char *str, char c)
{
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
        
        if (row == 0)
            vars->point_map.width = cols;
        
        vars->point_map.points[row] = malloc(cols * sizeof(t_point));
        if (!vars->point_map.points[row])
        {
            free_points(vars);
            return (0);
        }
        
        unsigned int col = 0;
        char *item_ref;
        char *item = strtok_r(line, " \t", &item_ref);
        
        while (item != NULL && col < cols)
        {
            char *color_str = strchr(item, ',');
            if(color_str)
                vars->point_map.points[row][col].color.hex = strtoul(color_str + 1, NULL, 0);
            else
                vars->point_map.points[row][col].color.hex = 0xFFFFFF;
            
            vars->point_map.points[row][col].x = col;
            vars->point_map.points[row][col].y = row;
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

// ============ FUNÇÕES DE MATRIZ ============

// Inicializa matriz identidade
void matrix_identity(t_matrix *m)
{
    int i, j;
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            m->m[i][j] = (i == j) ? 1.0 : 0.0;
}

// Multiplica duas matrizes
t_matrix matrix_multiply(t_matrix a, t_matrix b)
{
    t_matrix result;
    int i, j, k;
    
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            result.m[i][j] = 0;
            for (k = 0; k < 3; k++)
                result.m[i][j] += a.m[i][k] * b.m[k][j];
        }
    }
    return result;
}

// Aplica matriz a um ponto
t_point matrix_apply(t_matrix m, t_point p)
{
    t_point result;
    result.x = m.m[0][0] * p.x + m.m[0][1] * p.y + m.m[0][2] * p.z;
    result.y = m.m[1][0] * p.x + m.m[1][1] * p.y + m.m[1][2] * p.z;
    result.z = m.m[2][0] * p.x + m.m[2][1] * p.y + m.m[2][2] * p.z;
    result.color = p.color;
    return result;
}

// Cria matriz de rotação em X
t_matrix matrix_rotation_x(double angle)
{
    t_matrix m;
    matrix_identity(&m);
    m.m[1][1] = cos(angle);
    m.m[1][2] = -sin(angle);
    m.m[2][1] = sin(angle);
    m.m[2][2] = cos(angle);
    return m;
}

// Cria matriz de rotação em Y
t_matrix matrix_rotation_y(double angle)
{
    t_matrix m;
    matrix_identity(&m);
    m.m[0][0] = cos(angle);
    m.m[0][2] = sin(angle);
    m.m[2][0] = -sin(angle);
    m.m[2][2] = cos(angle);
    return m;
}

// Cria matriz de rotação em Z
t_matrix matrix_rotation_z(double angle)
{
    t_matrix m;
    matrix_identity(&m);
    m.m[0][0] = cos(angle);
    m.m[0][1] = -sin(angle);
    m.m[1][0] = sin(angle);
    m.m[1][1] = cos(angle);
    return m;
}

// Cria matriz de rotação ao redor de um eixo arbitrário (Fórmula de Rodrigues)
t_matrix matrix_rotation_axis(double axis_x, double axis_y, double axis_z, double angle)
{
    t_matrix m;
    double c = cos(angle);
    double s = sin(angle);
    double t = 1.0 - c;
    
    // Normaliza o eixo
    double len = sqrt(axis_x * axis_x + axis_y * axis_y + axis_z * axis_z);
    double x = axis_x / len;
    double y = axis_y / len;
    double z = axis_z / len;
    
    // Fórmula de Rodrigues
    m.m[0][0] = t * x * x + c;
    m.m[0][1] = t * x * y - s * z;
    m.m[0][2] = t * x * z + s * y;
    
    m.m[1][0] = t * x * y + s * z;
    m.m[1][1] = t * y * y + c;
    m.m[1][2] = t * y * z - s * x;
    
    m.m[2][0] = t * x * z - s * y;
    m.m[2][1] = t * y * z + s * x;
    m.m[2][2] = t * z * z + c;
    
    return m;
}

// ============ FUNÇÕES DE ROTAÇÃO ANTIGAS (mantidas para compatibilidade) ============

// Rotação em torno do eixo X (olhando da direita para a esquerda)
t_point rotate_x(t_point p, double angle)
{
    t_point rotated;
    rotated.x = p.x;
    rotated.y = p.y * cos(angle) + p.z * sin(angle);
    rotated.z = -p.y * sin(angle) + p.z * cos(angle);
    rotated.color = p.color;
    return rotated;
}

// Rotação em torno do eixo Y (olhando de cima para baixo)
t_point rotate_y(t_point p, double angle)
{
    t_point rotated;
    rotated.x = p.x * cos(angle) - p.z * sin(angle);
    rotated.y = p.y;
    rotated.z = p.x * sin(angle) + p.z * cos(angle);
    rotated.color = p.color;
    return rotated;
}

// Rotação em torno do eixo Z (olhando de frente)
t_point rotate_z(t_point p, double angle)
{
    t_point rotated;
    rotated.x = p.x * cos(angle) - p.y * sin(angle);
    rotated.y = p.x * sin(angle) + p.y * cos(angle);
    rotated.z = p.z;
    rotated.color = p.color;
    return rotated;
}

// Aplica todas as rotações na ordem correta para rotação relativa à câmera
t_point apply_rotations(t_point p, t_rotation rot)
{
    t_point result = p;
    // A ordem importa! Aplicamos Z, depois Y, depois X para rotação tipo trackball
    result = rotate_z(result, rot.z);
    result = rotate_y(result, rot.y);
    result = rotate_x(result, rot.x);
    return result;
}

// Projeção isométrica
t_point project_iso(t_point p, t_vars *vars)
{
    t_point projected;
    double angle = M_PI / 6;
    double z_scale = 0.5;
    
    // Centraliza o modelo no seu próprio centro
    t_point centered;
    centered.x = p.x - (vars->point_map.width / 2.0);
    centered.y = p.y - (vars->point_map.height / 2.0);
    centered.z = p.z;
    centered.color = p.color;
    
    // Aplica a matriz de rotação acumulada
    t_point rotated = matrix_apply(vars->rot_matrix, centered);
    
    // Projeção isométrica
    double iso_x = (rotated.x - rotated.y) * cos(angle);
    double iso_y = (rotated.x + rotated.y) * sin(angle) - (rotated.z * z_scale);
    
    // Aplica zoom e converte para coordenadas de tela com translação
    double scale = vars->screen.size * vars->zoom;
    projected.x = vars->screen.offset_x + iso_x * scale + vars->translation.x;
    projected.y = vars->screen.offset_y + iso_y * scale + vars->translation.y;
    projected.z = rotated.z;
    projected.color = p.color;
    
    return projected;
}

void put_pixel(t_vars *vars, int x, int y, int color)
{
    if (x >= 0 && x < vars->screen.width && y >= 0 && y < vars->screen.height)
    {
        char *dst = vars->screen.addr + (y * vars->screen.line_length + x * (vars->screen.bits_per_pixel / 8));
        *(unsigned int*)dst = color;
    }
}

void draw_line(t_point p0, t_point p1, t_vars *vars)
{
    int dx = abs((int)p1.x - (int)p0.x);
    int dy = abs((int)p1.y - (int)p0.y);
    int sx = (p0.x < p1.x) ? 1 : -1;
    int sy = (p0.y < p1.y) ? 1 : -1;
    int err = dx - dy;
    int x = (int)p0.x;
    int y = (int)p0.y;
    
    while (1)
    {
        put_pixel(vars, x, y, p0.color.hex);
        
        if (x == (int)p1.x && y == (int)p1.y)
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

int render(t_vars *vars)
{
    // Limpa a imagem
    memset(vars->screen.addr, 0, vars->screen.height * vars->screen.line_length);
    
    // Desenha o wireframe
    for (unsigned int row = 0; row < vars->point_map.height; row++)
    {
        for (unsigned int col = 0; col < vars->point_map.width; col++)
        {
            t_point p_atual = vars->point_map.points[row][col];
            t_point proj_atual = project_iso(p_atual, vars);
            
            if (col + 1 < vars->point_map.width)
            {
                t_point p_direita = vars->point_map.points[row][col + 1];
                t_point proj_direita = project_iso(p_direita, vars);
                draw_line(proj_atual, proj_direita, vars);
            }
            
            if (row + 1 < vars->point_map.height)
            {
                t_point p_abaixo = vars->point_map.points[row + 1][col];
                t_point proj_abaixo = project_iso(p_abaixo, vars);
                draw_line(proj_atual, proj_abaixo, vars);
            }
        }
    }
    
    // Coloca a imagem na janela
    mlx_put_image_to_window(vars->mlx, vars->win, vars->screen.img, 0, 0);
    
    // Mostra controles
    mlx_string_put(vars->mlx, vars->win, 10, 20, 0xFFFFFF, "WASD: Rot X/Y | Q/E: Rot Z | Setas: Mover | +/-: Zoom | ESC: Sair");
    
    return (0);
}

int handle_keypress(int keycode, t_vars *vars)
{
    if (keycode == KEY_ESC)
        exit(0);
    
    // Marca teclas como pressionadas
    else if (keycode == KEY_W)
        vars->keys.w = 1;
    else if (keycode == KEY_S)
        vars->keys.s = 1;
    else if (keycode == KEY_A)
        vars->keys.a = 1;
    else if (keycode == KEY_D)
        vars->keys.d = 1;
    else if (keycode == KEY_Q)
        vars->keys.q = 1;
    else if (keycode == KEY_E)
        vars->keys.e = 1;
    else if (keycode == KEY_UP)
        vars->keys.up = 1;
    else if (keycode == KEY_DOWN)
        vars->keys.down = 1;
    else if (keycode == KEY_LEFT)
        vars->keys.left = 1;
    else if (keycode == KEY_RIGHT)
        vars->keys.right = 1;
    else if (keycode == KEY_PLUS)
        vars->keys.plus = 1;
    else if (keycode == KEY_MINUS)
        vars->keys.minus = 1;
    
    return (0);
}

int handle_keyrelease(int keycode, t_vars *vars)
{
    // Marca teclas como não pressionadas
    if (keycode == KEY_W)
        vars->keys.w = 0;
    else if (keycode == KEY_S)
        vars->keys.s = 0;
    else if (keycode == KEY_A)
        vars->keys.a = 0;
    else if (keycode == KEY_D)
        vars->keys.d = 0;
    else if (keycode == KEY_Q)
        vars->keys.q = 0;
    else if (keycode == KEY_E)
        vars->keys.e = 0;
    else if (keycode == KEY_UP)
        vars->keys.up = 0;
    else if (keycode == KEY_DOWN)
        vars->keys.down = 0;
    else if (keycode == KEY_LEFT)
        vars->keys.left = 0;
    else if (keycode == KEY_RIGHT)
        vars->keys.right = 0;
    else if (keycode == KEY_PLUS)
        vars->keys.plus = 0;
    else if (keycode == KEY_MINUS)
        vars->keys.minus = 0;
    
    return (0);
}

int update_and_render(t_vars *vars)
{
    double step = 0.01;  // sensibilidade para rotação
    double move_step = 5.0;  // pixels para translação
    double zoom_step = 0.02;  // incremento de zoom
    
    // Extrai os vetores da câmera a partir da matriz de rotação atual
    // As COLUNAS da matriz representam os eixos transformados:
    // Coluna 0 = eixo X transformado
    // Coluna 1 = eixo Y transformado  
    // Coluna 2 = eixo Z transformado (perpendicular à tela na vista isométrica)
    
    // Q/E: rola ao redor do eixo Z da câmera (perpendicular à tela)
    if (vars->keys.q)
    {
        // Rotaciona ao redor do eixo Z atual da câmera
        t_matrix rot = matrix_rotation_axis(
            vars->rot_matrix.m[0][2],  // Z_x da câmera
            vars->rot_matrix.m[1][2],  // Z_y da câmera
            vars->rot_matrix.m[2][2],  // Z_z da câmera
            -step
        );
        vars->rot_matrix = matrix_multiply(rot, vars->rot_matrix);
    }
    if (vars->keys.e)
    {
        t_matrix rot = matrix_rotation_axis(
            vars->rot_matrix.m[0][2],
            vars->rot_matrix.m[1][2],
            vars->rot_matrix.m[2][2],
            step
        );
        vars->rot_matrix = matrix_multiply(rot, vars->rot_matrix);
    }
    
    // A/D: roda ao redor do eixo Y da câmera (vertical da tela)
    if (vars->keys.a)
    {
        t_matrix rot = matrix_rotation_axis(
            vars->rot_matrix.m[0][1],  // Y_x da câmera
            vars->rot_matrix.m[1][1],  // Y_y da câmera
            vars->rot_matrix.m[2][1],  // Y_z da câmera
            -step
        );
        vars->rot_matrix = matrix_multiply(rot, vars->rot_matrix);
    }
    if (vars->keys.d)
    {
        t_matrix rot = matrix_rotation_axis(
            vars->rot_matrix.m[0][1],
            vars->rot_matrix.m[1][1],
            vars->rot_matrix.m[2][1],
            step
        );
        vars->rot_matrix = matrix_multiply(rot, vars->rot_matrix);
    }
    
    // W/S: roda ao redor do eixo X da câmera (horizontal da tela)
    if (vars->keys.s)
    {
        t_matrix rot = matrix_rotation_axis(
            vars->rot_matrix.m[0][0],  // X_x da câmera
            vars->rot_matrix.m[1][0],  // X_y da câmera
            vars->rot_matrix.m[2][0],  // X_z da câmera
            -step
        );
        vars->rot_matrix = matrix_multiply(rot, vars->rot_matrix);
    }
    if (vars->keys.w)
    {
        t_matrix rot = matrix_rotation_axis(
            vars->rot_matrix.m[0][0],
            vars->rot_matrix.m[1][0],
            vars->rot_matrix.m[2][0],
            step
        );
        vars->rot_matrix = matrix_multiply(rot, vars->rot_matrix);
    }
    
    // Aplica translações baseado nas teclas pressionadas
    if (vars->keys.up)
        vars->translation.y -= move_step;
    if (vars->keys.down)
        vars->translation.y += move_step;
    if (vars->keys.left)
        vars->translation.x -= move_step;
    if (vars->keys.right)
        vars->translation.x += move_step;
    
    // Aplica zoom
    if (vars->keys.plus)
    {
        vars->zoom += zoom_step;
        if (vars->zoom > 10.0)  // limite máximo
            vars->zoom = 10.0;
    }
    if (vars->keys.minus)
    {
        vars->zoom -= zoom_step;
        if (vars->zoom < 0.1)  // limite mínimo
            vars->zoom = 0.1;
    }
    
    // Redesenha
    render(vars);
    
    return (0);
}

int close_window(t_vars *vars)
{
    (void)vars;
    exit(0);
    return (0);
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "usage: %s <file.fdf>\n", argv[0]);
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
    memset(&vars, 0, sizeof(t_vars));
    
    if(!try_parse(&vars, content))
    {
        printf("Erro: falha ao analisar o mapa\n");
        free(content);
        close(fd);
        return (1);
    }
    
    vars.mlx = mlx_init();    
    if (!vars.mlx)
    {
        printf("Erro: mlx_init falhou\n");
        return 1;
    }
    
    // Tamanho da janela
    int screen_w, screen_h;
    mlx_get_screen_size(vars.mlx, &screen_w, &screen_h);
    if (screen_w > 2560) screen_w = 2560;
    if (screen_h > 1440) screen_h = 1440;
    
    vars.screen.width = screen_w * 0.8;
    vars.screen.height = screen_h * 0.8;
    
    // Calcula escala
    double usable_width = vars.screen.width * 0.85;
    double usable_height = vars.screen.height * 0.85;
    double projected_width = vars.point_map.width * 0.866 + vars.point_map.height * 0.866;
    double projected_height = vars.point_map.width * 0.5 + vars.point_map.height * 0.5;
    double scale_x = usable_width / projected_width;
    double scale_y = usable_height / projected_height;
    vars.screen.size = (scale_x < scale_y) ? scale_x : scale_y;
    
    vars.screen.offset_x = vars.screen.width / 2;
    vars.screen.offset_y = vars.screen.height / 2.5;
    
    // Inicializa matriz de rotação como identidade
    matrix_identity(&vars.rot_matrix);
    
    // Rotação inicial (isométrico padrão) - ainda mantém para compatibilidade
    vars.rotation.x = 0;
    vars.rotation.y = 0;
    vars.rotation.z = 0;
    
    // Translação inicial
    vars.translation.x = 0;
    vars.translation.y = 0;
    
    // Zoom inicial
    vars.zoom = 1.0;
    
    // Inicializa estado das teclas
    memset(&vars.keys, 0, sizeof(t_keys));
    
    vars.win = mlx_new_window(vars.mlx, vars.screen.width, vars.screen.height, "FDF Interactive - Use WASD/QE/Arrows");
    if (!vars.win)
    {
        printf("Erro: mlx_new_window falhou\n");
        return 1;
    }
    
    // Cria imagem para double buffering
    vars.screen.img = mlx_new_image(vars.mlx, vars.screen.width, vars.screen.height);
    vars.screen.addr = mlx_get_data_addr(vars.screen.img, &vars.screen.bits_per_pixel,
                                          &vars.screen.line_length, &vars.screen.endian);
    
    // Hooks
    mlx_hook(vars.win, 17, 0, close_window, &vars);
    mlx_hook(vars.win, 2, 1L<<0, handle_keypress, &vars);     // KeyPress event
    mlx_hook(vars.win, 3, 1L<<1, handle_keyrelease, &vars);   // KeyRelease event
    mlx_loop_hook(vars.mlx, update_and_render, &vars);         // Loop contínuo
    
    // Render inicial
    render(&vars);
    
    mlx_loop(vars.mlx);
    
    free(content);
    close(fd);
    return 0;
}
