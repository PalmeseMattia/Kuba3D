#ifndef CUBE_ANIMATIONS_H
# define CUBE_ANIMATIONS_H

# define ANIM_ENEMY_IDLE_FRAME_0 "assets/textures/biohazard/xpm/enemies/enemy_1.xpm"
# define ANIM_ENEMY_IDLE_FRAME_1 "assets/textures/biohazard/xpm/enemies/enemy_2.xpm"
# define ANIM_ENEMY_IDLE_FRAME_2 "assets/textures/biohazard/xpm/enemies/enemy_3.xpm"

typedef enum	s_animation_type
{
	ANIM_TYPE_IDLE,
	ANIM_TYPE_ATTACK,
	ANIM_TYPE_RELOAD,
	ANIM_TYPE_TAKE_DAMAGE,
	ANIM_TYPE_DIE,
	ANIM_TYPE_OPEN
}	t_animation_type;

# include <stdlib.h>

typedef struct	s_animation
{
	t_animation_type	type;
	int					frame;
	size_t				*sprites_ptr;
}	t_animation;

typedef struct	s_animation_controller
{
	t_animation	*idle;
	t_animation	*attack;
	t_animation	*reload;
	t_animation	*take_damage;
	t_animation	*die;
	t_animation	*open;
}	t_animation_controller;

t_animation				*anim_animation_init(t_animation_type type, size_t *sprites_ptr);
void					anim_animation_free(t_animation *animation);
t_animation_controller	*anim_animation_controller_init();
void					anim_animation_controller_free(t_animation_controller *animation_controller);
void					anim_animation_controller_set_animation(
	t_animation_controller *animation_controller, 
	t_animation_type type, size_t *sprites_ptr);

#endif