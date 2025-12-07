#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "mlx.h"








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
