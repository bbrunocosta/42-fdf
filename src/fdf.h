/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 19:17:46 by bcosta-b          #+#    #+#             */
/*   Updated: 2026/01/05 12:19:15 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define M_PI 3.14159265358979323846

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define MARGIN 100

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
#define KEY_PLUS 61
#define KEY_MINUS 45

typedef union u_color {
	unsigned int	hex;
	struct {
		unsigned char	r;
		unsigned char	g;
		unsigned char	b;
		unsigned char	a;
	};
}	t_color;





typedef struct s_point {
	double	x;
	double	y;
	double	z;
	t_color	color;
}	t_point;

typedef struct s_quaternion {
	double w;
	double x;
	double y;
	double z;
}	t_quaternion;

typedef struct s_rotation {
	double x;
	double y;
	double z;
}	t_rotation;

typedef struct s_translation {
	double x;
	double y;
}	t_translation;

typedef struct s_screen {
	double	width;
	double	height;
	double	offset_x;
	double	offset_y;
	double	size;
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}	t_screen;

typedef struct s_point_map {
	unsigned int	width;
	unsigned int	height;
	t_point			**points;
}	t_point_map;

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
}	t_keys;

typedef struct s_vars {
	void			*mlx;
	void			*win;
	t_screen		screen;
	t_point_map		point_map;
	t_rotation		rotation;
	t_translation	translation;
	t_keys			keys;
	double			zoom;
	t_quaternion	quat;
	int				needs_render;
}	t_vars;


t_point			quat_rotate_point(t_quaternion q, t_point p);
t_quaternion	quat_multiply(t_quaternion q1, t_quaternion q2);
void			quat_to_axes(t_quaternion q, double *x_axis, double *y_axis, double *z_axis);
t_quaternion	quat_identity(void);
t_quaternion	quat_from_axis_angle(double axis_x, double axis_y, double axis_z, double angle);
t_quaternion	quat_conjugate(t_quaternion q);
t_quaternion	quat_normalize(t_quaternion q);

int				count_char(const char *str, char c);
int				count_tokens(const char *str);
int				free_points(t_vars *vars);
int				try_parse(t_vars *vars, char *map);
char			*read_file_to_string(int fd);
int				parse_map(t_vars *vars, char *filename);

void			draw_line_optimized(t_vars *vars, t_point p1, t_point p2);
void			put_pixel(t_vars *vars, int x, int y, t_color color);
void			draw_line(t_vars *vars, t_point p1, t_point p2);
void			render(t_vars *vars);
int				is_visible(t_point p, t_vars *vars);

t_point			project_iso(t_point p, t_vars *vars);

int				handle_keypress(int keycode, t_vars *vars);
int				handle_keyrelease(int keycode, t_vars *vars);
int				close_window(t_vars *vars);
int				update_and_render(t_vars *vars);
void			update_rotation(t_vars *vars, int *changed);
void			update_rotation_x(t_vars *vars, int *changed);
void			update_translation(t_vars *vars, int *changed);
void			update_zoom(t_vars *vars, int *changed);

void			init_screen_size(t_vars *vars);
void			init_vars(t_vars *vars);
int				setup_mlx(t_vars *vars);

void			ft_putstr_fd(char *s, int fd);
void			*ft_memset(void *s, int c, size_t n);
int				ft_atoi(const char *str);
char			*ft_strchr(const char *s, int c);
char			*ft_strdup(const char *s);
unsigned int	ft_atoi_base(const char *str, int base);
char			*ft_skip_spaces(char *str);
char			**ft_split(char const *s, char c);
char			*ft_strjoin(char const *s1, char const *s2);

#endif