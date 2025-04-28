#include <cube_animations.h>
#include <utils.h>

t_animation	*anim_animation_init(t_animation_type type, size_t *sprites_ptr)
{
	t_animation	*animation;

	animation = malloc(sizeof(t_animation));
	if (!animation)
		return (NULL);
	animation->frame = 0;
	animation->type = type;
	animation->sprites_ptr = sprites_ptr;
	return (animation);
}

void	anim_animation_free(t_animation *animation)
{
	safe_free(animation);
	animation = NULL;
}

t_animation_controller	*anim_animation_controller_init()
{
	t_animation_controller	*animation_controller;

	animation_controller = malloc(sizeof(t_animation_controller));
	if (!animation_controller)
		return (NULL);
	animation_controller->attack = NULL;
	animation_controller->die = NULL;
	animation_controller->idle = NULL;
	animation_controller->open = NULL;
	animation_controller->reload = NULL;
	animation_controller->take_damage = NULL;
	return (animation_controller);
}

void	anim_animation_controller_free(t_animation_controller *animation_controller)
{
	safe_free(animation_controller);
	animation_controller = NULL;
}

void	anim_animation_controller_set_animation(
	t_animation_controller *animation_controller, 
	t_animation_type type, size_t *sprites_ptr)
{
	if (!animation_controller || !sprites_ptr)
		return ;
	if (type == ANIM_TYPE_ATTACK)
		animation_controller->attack = anim_animation_init(type, sprites_ptr);
	else if (type == ANIM_TYPE_DIE)
		animation_controller->die = anim_animation_init(type, sprites_ptr);
	else if (type == ANIM_TYPE_IDLE)
		animation_controller->idle = anim_animation_init(type, sprites_ptr);
	else if (type == ANIM_TYPE_OPEN)
		animation_controller->open = anim_animation_init(type, sprites_ptr);
	else if (type == ANIM_TYPE_RELOAD)
		animation_controller->reload = anim_animation_init(type, sprites_ptr);
	else if (type == ANIM_TYPE_TAKE_DAMAGE)
		animation_controller->take_damage = anim_animation_init(type, sprites_ptr);
}