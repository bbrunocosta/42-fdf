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

// ========== QUATERNION ==========
typedef struct s_quaternion {
    double w;  // parte escalar (real)
    double x;  // parte vetorial i
    double y;  // parte vetorial j
    double z;  // parte vetorial k
} t_quaternion;

typedef struct s_rotation {
    double x;
    double y;
    double z;
} t_rotation;

typedef struct s_translation {
    double x;
    double y;
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
    t_quaternion quat;  // Quaternion de rotação acumulada
    int needs_render;  // Flag para renderização condicional
} t_vars;

// ========== FUNÇÕES DE QUATERNION ==========

// Cria quaternion identidade (sem rotação)
t_quaternion quat_identity(void)
{
    t_quaternion q;
    q.w = 1.0;
    q.x = 0.0;
    q.y = 0.0;
    q.z = 0.0;
    return q;
}

// Cria quaternion a partir de ângulo e eixo
t_quaternion quat_from_axis_angle(double axis_x, double axis_y, double axis_z, double angle)
{
    t_quaternion q;
    double half_angle = angle / 2.0;
    double sin_half = sin(half_angle);
    
    // Normaliza o eixo
    double len = sqrt(axis_x * axis_x + axis_y * axis_y + axis_z * axis_z);
    if (len < 0.0001)  // evita divisão por zero
        return quat_identity();
    
    axis_x /= len;
    axis_y /= len;
    axis_z /= len;
    
    q.w = cos(half_angle);
    q.x = axis_x * sin_half;
    q.y = axis_y * sin_half;
    q.z = axis_z * sin_half;
    
    return q;
}

// Multiplica dois quaternions (combina rotações)
// q1 * q2 significa: primeiro aplica q2, depois q1
t_quaternion quat_multiply(t_quaternion q1, t_quaternion q2)
{
    t_quaternion result;
    
    result.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
    result.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
    result.y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x;
    result.z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w;
    
    return result;
}

// Normaliza quaternion (garante que |q| = 1)
t_quaternion quat_normalize(t_quaternion q)
{
    double len = sqrt(q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z);
    
    if (len < 0.0001)
        return quat_identity();
    
    q.w /= len;
    q.x /= len;
    q.y /= len;
    q.z /= len;
    
    return q;
}

// Conjudado de quaternion (inverte a rotação)
t_quaternion quat_conjugate(t_quaternion q)
{
    t_quaternion conj;
    conj.w = q.w;
    conj.x = -q.x;
    conj.y = -q.y;
    conj.z = -q.z;
    return conj;
}

// Aplica rotação de quaternion a um ponto 3D
t_point quat_rotate_point(t_quaternion q, t_point p)
{
    // Converte ponto para quaternion puro (w=0)
    t_quaternion p_quat;
    p_quat.w = 0;
    p_quat.x = p.x;
    p_quat.y = p.y;
    p_quat.z = p.z;
    
    // Aplica rotação: p' = q * p * q^-1
    t_quaternion q_conj = quat_conjugate(q);
    t_quaternion temp = quat_multiply(q, p_quat);
    t_quaternion result_quat = quat_multiply(temp, q_conj);
    
    // Converte de volta para ponto
    t_point result;
    result.x = result_quat.x;
    result.y = result_quat.y;
    result.z = result_quat.z;
    result.color = p.color;
    
    return result;
}

// Extrai vetores de eixo a partir do quaternion
void quat_to_axes(t_quaternion q, double *x_axis, double *y_axis, double *z_axis)
{
    // Eixo X (coluna 0 da matriz de rotação)
    x_axis[0] = 1 - 2 * (q.y * q.y + q.z * q.z);
    x_axis[1] = 2 * (q.x * q.y + q.w * q.z);
    x_axis[2] = 2 * (q.x * q.z - q.w * q.y);
    
    // Eixo Y (coluna 1 da matriz de rotação)
    y_axis[0] = 2 * (q.x * q.y - q.w * q.z);
    y_axis[1] = 1 - 2 * (q.x * q.x + q.z * q.z);
    y_axis[2] = 2 * (q.y * q.z + q.w * q.x);
    
    // Eixo Z (coluna 2 da matriz de rotação)
    z_axis[0] = 2 * (q.x * q.z + q.w * q.y);
    z_axis[1] = 2 * (q.y * q.z - q.w * q.x);
    z_axis[2] = 1 - 2 * (q.x * q.x + q.y * q.y);
}

// ========== FUNÇÕES DE PARSING ==========

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

int count_tokens(const char *str)
{
    int count = 0;
    int in_token = 0;
    
    while (*str)
    {
        if (*str != ' ' && *str != '\t' && *str != '\n')
        {
            if (!in_token)
            {
                count++;
                in_token = 1;
            }
        }
        else
        {
            in_token = 0;
        }
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

char *read_file_to_string(int fd)
{
    char buffer[BUFFER_SIZE];
    char *content = malloc(1);
    if (!content)
        return NULL;
    content[0] = '\0';
    
    size_t total_size = 0;
    ssize_t bytesRead;
    
    while ((bytesRead = read(fd, buffer, BUFFER_SIZE - 1)) > 0)
    {
        buffer[bytesRead] = '\0';
        total_size += bytesRead;
        
        char *new_content = realloc(content, total_size + 1);
        if (!new_content)
        {
            free(content);
            return NULL;
        }
        content = new_content;
        strcat(content, buffer);
    }
    
    if (bytesRead < 0) {
        free(content);
        return NULL;
    }

    return content;
}

// ========== FUNÇÕES DE RENDERIZAÇÃO OTIMIZADAS ==========

// OTIMIZAÇÃO 1: Frustum Culling - verifica se ponto está visível
static inline int is_visible(t_point p, t_vars *vars)
{
    int margin = 100;  // margem extra para evitar cortes
    return (p.x >= -margin && p.x < vars->screen.width + margin &&
            p.y >= -margin && p.y < vars->screen.height + margin);
}

// OTIMIZAÇÃO 2: Put pixel otimizado (inline)
static inline void put_pixel_fast(t_vars *vars, int x, int y, unsigned int color)
{
    if (x < 0 || x >= (int)vars->screen.width || y < 0 || y >= (int)vars->screen.height)
        return;
    
    // Cache de valores para evitar cálculos repetidos
    int bpp = vars->screen.bits_per_pixel >> 3;  // divisão por 8 usando shift
    *(unsigned int*)(vars->screen.addr + (y * vars->screen.line_length + x * bpp)) = color;
}

// OTIMIZAÇÃO 3: Bresenham otimizado com frustum culling
void draw_line_optimized(t_vars *vars, t_point p1, t_point p2)
{
    // Frustum culling: se ambos pontos estão fora, não desenha
    if (!is_visible(p1, vars) && !is_visible(p2, vars))
        return;
    
    int dx = abs((int)p2.x - (int)p1.x);
    int dy = abs((int)p2.y - (int)p1.y);
    int sx = p1.x < p2.x ? 1 : -1;
    int sy = p1.y < p2.y ? 1 : -1;
    int err = dx - dy;
    
    int x = (int)p1.x;
    int y = (int)p1.y;
    int end_x = (int)p2.x;
    int end_y = (int)p2.y;
    
    unsigned int color = p1.color.hex;
    
    // Cache para otimização
    int bpp = vars->screen.bits_per_pixel >> 3;
    int line_len = vars->screen.line_length;
    int max_x = (int)vars->screen.width;
    int max_y = (int)vars->screen.height;
    
    while (1)
    {
        // Verifica bounds inline
        if (x >= 0 && x < max_x && y >= 0 && y < max_y)
        {
            *(unsigned int*)(vars->screen.addr + (y * line_len + x * bpp)) = color;
        }
        
        if (x == end_x && y == end_y)
            break;
        
        int e2 = err << 1;  // multiplicação por 2 usando shift
        
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

// ========== FUNÇÕES DE RENDERIZAÇÃO ==========

void put_pixel(t_vars *vars, int x, int y, t_color color)
{
    if (x < 0 || x >= vars->screen.width || y < 0 || y >= vars->screen.height)
        return;
    
    char *dst = vars->screen.addr + (y * vars->screen.line_length + x * (vars->screen.bits_per_pixel / 8));
    *(unsigned int*)dst = color.hex;
}

void draw_line(t_vars *vars, t_point p1, t_point p2)
{
    int dx = abs((int)p2.x - (int)p1.x);
    int dy = abs((int)p2.y - (int)p1.y);
    int sx = p1.x < p2.x ? 1 : -1;
    int sy = p1.y < p2.y ? 1 : -1;
    int err = dx - dy;
    
    int x = (int)p1.x;
    int y = (int)p1.y;
    
    while (1)
    {
        put_pixel(vars, x, y, p1.color);
        
        if (x == (int)p2.x && y == (int)p2.y)
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

// Projeção isométrica com quaternion
t_point project_iso(t_point p, t_vars *vars)
{
    t_point projected;
    double angle = M_PI / 6;
    double z_scale = 0.5;
    
    // Centraliza o modelo
    t_point centered;
    centered.x = p.x - (vars->point_map.width / 2.0);
    centered.y = p.y - (vars->point_map.height / 2.0);
    centered.z = p.z;
    centered.color = p.color;
    
    // Aplica rotação usando quaternion
    t_point rotated = quat_rotate_point(vars->quat, centered);
    
    // Projeção isométrica
    double iso_x = (rotated.x - rotated.y) * cos(angle);
    double iso_y = (rotated.x + rotated.y) * sin(angle) - (rotated.z * z_scale);
    
    // Aplica zoom e translação
    double scale = vars->screen.size * vars->zoom;
    projected.x = vars->screen.offset_x + iso_x * scale + vars->translation.x;
    projected.y = vars->screen.offset_y + iso_y * scale + vars->translation.y;
    projected.z = rotated.z;
    projected.color = p.color;
    
    return projected;
}

// OTIMIZAÇÃO 4: Renderização com LOD (Level of Detail)
void render(t_vars *vars)
{
    // Limpa a imagem
    memset(vars->screen.addr, 0, vars->screen.height * vars->screen.line_length);
    
    // LOD: calcula step baseado no zoom
    unsigned int step = 1;
    if (vars->zoom < 0.3)
        step = 4;  // Muito longe: desenha 1 em cada 4 linhas
    else if (vars->zoom < 0.6)
        step = 2;  // Longe: desenha 1 em cada 2 linhas
    // zoom >= 0.6: desenha tudo (step = 1)
    
    // Desenha linhas conectando os pontos
    for (unsigned int y = 0; y < vars->point_map.height; y += step)
    {
        for (unsigned int x = 0; x < vars->point_map.width; x += step)
        {
            t_point p1 = project_iso(vars->point_map.points[y][x], vars);
            
            // Conecta com o ponto à direita
            if (x + step < vars->point_map.width)
            {
                t_point p2 = project_iso(vars->point_map.points[y][x + step], vars);
                draw_line_optimized(vars, p1, p2);
            }
            
            // Conecta com o ponto abaixo
            if (y + step < vars->point_map.height)
            {
                t_point p2 = project_iso(vars->point_map.points[y + step][x], vars);
                draw_line_optimized(vars, p1, p2);
            }
        }
    }
    
    // Coloca a imagem na janela
    mlx_put_image_to_window(vars->mlx, vars->win, vars->screen.img, 0, 0);
}

// ========== INPUT E EVENTOS ==========

int handle_keypress(int keycode, t_vars *vars)
{
    if (keycode == KEY_ESC)
        exit(0);
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

// OTIMIZAÇÃO 5: Renderização condicional
int update_and_render(t_vars *vars)
{
    double step = 0.01;
    double move_step = 5.0;
    double zoom_step = 0.02;
    int changed = 0;  // Flag para detectar mudanças
    
    // Extrai os eixos atuais do quaternion
    double x_axis[3], y_axis[3], z_axis[3];
    quat_to_axes(vars->quat, x_axis, y_axis, z_axis);
    
    // Q/E: rola ao redor do eixo Z da câmera
    if (vars->keys.q)
    {
        t_quaternion rot = quat_from_axis_angle(z_axis[0], z_axis[1], z_axis[2], -step);
        vars->quat = quat_multiply(rot, vars->quat);
        vars->quat = quat_normalize(vars->quat);  // importante: renormalizar!
        changed = 1;
    }
    if (vars->keys.e)
    {
        t_quaternion rot = quat_from_axis_angle(z_axis[0], z_axis[1], z_axis[2], step);
        vars->quat = quat_multiply(rot, vars->quat);
        vars->quat = quat_normalize(vars->quat);
        changed = 1;
    }
    
    // A/D: roda ao redor do eixo Y da câmera (vertical da tela)
    if (vars->keys.a)
    {
        t_quaternion rot = quat_from_axis_angle(y_axis[0], y_axis[1], y_axis[2], -step);
        vars->quat = quat_multiply(rot, vars->quat);
        vars->quat = quat_normalize(vars->quat);
        changed = 1;
    }
    if (vars->keys.d)
    {
        t_quaternion rot = quat_from_axis_angle(y_axis[0], y_axis[1], y_axis[2], step);
        vars->quat = quat_multiply(rot, vars->quat);
        vars->quat = quat_normalize(vars->quat);
        changed = 1;
    }
    
    // W/S: roda ao redor do eixo X da câmera (horizontal da tela)
    if (vars->keys.s)
    {
        t_quaternion rot = quat_from_axis_angle(x_axis[0], x_axis[1], x_axis[2], -step);
        vars->quat = quat_multiply(rot, vars->quat);
        vars->quat = quat_normalize(vars->quat);
        changed = 1;
    }
    if (vars->keys.w)
    {
        t_quaternion rot = quat_from_axis_angle(x_axis[0], x_axis[1], x_axis[2], step);
        vars->quat = quat_multiply(rot, vars->quat);
        vars->quat = quat_normalize(vars->quat);
        changed = 1;
    }
    
    // Translações
    if (vars->keys.up)
    {
        vars->translation.y -= move_step;
        changed = 1;
    }
    if (vars->keys.down)
    {
        vars->translation.y += move_step;
        changed = 1;
    }
    if (vars->keys.left)
    {
        vars->translation.x -= move_step;
        changed = 1;
    }
    if (vars->keys.right)
    {
        vars->translation.x += move_step;
        changed = 1;
    }
    
    // Zoom
    if (vars->keys.plus)
    {
        vars->zoom += zoom_step;
        if (vars->zoom > 10.0)
            vars->zoom = 10.0;
        changed = 1;
    }
    if (vars->keys.minus)
    {
        vars->zoom -= zoom_step;
        if (vars->zoom < 0.1)
            vars->zoom = 0.1;
        changed = 1;
    }
    
    // RENDERIZAÇÃO CONDICIONAL: só renderiza se houve mudança ou é o primeiro frame
    if (changed || vars->needs_render)
    {
        render(vars);
        vars->needs_render = 0;
    }
    
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
    
    // Inicializa quaternion como identidade (sem rotação)
    vars.quat = quat_identity();
    
    // Rotação inicial (não usada, mantida para compatibilidade)
    vars.rotation.x = 0;
    vars.rotation.y = 0;
    vars.rotation.z = 0;
    
    // Translação inicial
    vars.translation.x = 0;
    vars.translation.y = 0;
    
    // Zoom inicial
    vars.zoom = 1.0;
    
    // Força render inicial
    vars.needs_render = 1;
    
    // Inicializa estado das teclas
    memset(&vars.keys, 0, sizeof(t_keys));
    
    vars.win = mlx_new_window(vars.mlx, vars.screen.width, vars.screen.height, "FDF Quaternion - WASD/QE/Arrows");
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
    mlx_hook(vars.win, 2, 1L<<0, handle_keypress, &vars);
    mlx_hook(vars.win, 3, 1L<<1, handle_keyrelease, &vars);
    mlx_loop_hook(vars.mlx, update_and_render, &vars);
    
    // Render inicial
    render(&vars);
    
    mlx_loop(vars.mlx);
    
    free(content);
    close(fd);
    return 0;
}
