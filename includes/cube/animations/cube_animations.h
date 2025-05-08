#ifndef CUBE_ANIMATIONS_H
# define CUBE_ANIMATIONS_H

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

typedef struct	s_animated_frames	t_animated_frames;

typedef struct	s_animation
{
	t_animation_type	type;
	int					frame;
	t_animated_frames	*frames_ptr;
}	t_animation;

#include <utils.h>

typedef struct	s_animation_controller
{
	t_animation	*idle;
	t_animation	*attack;
	t_animation	*reload;
	t_animation	*take_damage;
	t_animation	*die;
	t_animation	*open;
	t_animation	*current;

	t_bool	playing;
	t_bool	repeat;
	t_bool	reverse;
}	t_animation_controller;

t_animation				*anim_animation_init(t_animation_type type, t_animated_frames *frames_ptr);
void					anim_animation_free(t_animation *animation);
t_animation_controller	*anim_animation_controller_init();
void					anim_animation_controller_free(t_animation_controller *animation_controller);
void					anim_animation_controller_set_animation(
	t_animation_controller *animation_controller, 
	t_animation_type type, t_animated_frames *frames_ptr
);
void	anim_animation_controller_player_start(
	t_animation_controller *controller,
	t_animation_type type,
	t_bool reverse
);
void	anim_animation_controller_player_stop(
	t_animation_controller *controller
);

#endif