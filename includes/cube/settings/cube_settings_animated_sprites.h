#ifndef CUBE_SETTINGS_ANIMATED_SPRITE_H
# define CUBE_SETTINGS_ANIMATED_SPRITE_H

# define ANIM_ENEMY_IDLE_FRAME_0 "assets/textures/biohazard/xpm/enemies/idle_0.xpm"
# define ANIM_ENEMY_IDLE_FRAME_1 "assets/textures/biohazard/xpm/enemies/idle_1.xpm"
# define ANIM_ENEMY_IDLE_FRAME_2 "assets/textures/biohazard/xpm/enemies/idle_2.xpm"
# define ANIM_ENEMY_IDLE_FRAME_3 "assets/textures/biohazard/xpm/enemies/idle_3.xpm"
# define ANIM_ENEMY_IDLE_FRAME_4 "assets/textures/biohazard/xpm/enemies/idle_4.xpm"
# define ANIM_ENEMY_IDLE_FRAME_5 "assets/textures/biohazard/xpm/enemies/idle_5.xpm"
# define ANIM_ENEMY_IDLE_FRAME_6 "assets/textures/biohazard/xpm/enemies/idle_6.xpm"
# define ANIM_ENEMY_FRAMES_COUNT 7

# define ANIM_EXIT_IDLE_FRAME_0 "assets/textures/biohazard/xpm/exit/00.xpm"
# define ANIM_EXIT_IDLE_FRAME_1 "assets/textures/biohazard/xpm/exit/10.xpm"
# define ANIM_EXIT_IDLE_FRAME_2 "assets/textures/biohazard/xpm/exit/20.xpm"
# define ANIM_EXIT_IDLE_FRAME_3 "assets/textures/biohazard/xpm/exit/30.xpm"
# define ANIM_EXIT_IDLE_FRAME_4 "assets/textures/biohazard/xpm/exit/40.xpm"
# define ANIM_EXIT_IDLE_FRAME_5 "assets/textures/biohazard/xpm/exit/50.xpm"
# define ANIM_EXIT_IDLE_FRAME_6 "assets/textures/biohazard/xpm/exit/60.xpm"
# define ANIM_EXIT_IDLE_FRAME_7 "assets/textures/biohazard/xpm/exit/70.xpm"
# define ANIM_EXIT_FRAMES_COUNT 8

# include <stdlib.h>

typedef struct	s_animated_frames
{
	size_t	**frames;
	int		frames_count;
}	t_animated_frames;

typedef struct s_mlx_handler	t_mlx_handler;

t_animated_frames	*settings_enemy_frames_init(t_mlx_handler *mlx_handler);
t_animated_frames	*settings_exit_frames_init(t_mlx_handler *mlx_handler);

#endif