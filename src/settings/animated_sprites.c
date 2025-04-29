#include <cube_settings_animated_sprites.h>
#include <cube_settings_textures.h>
#include <cube.h>
#include <cube_mlx_handler.h>
#include <utils.h>

t_animated_frames	*settings_enemy_frames_init(t_mlx_handler *mlx_handler)
{
	t_animated_frames	*enemy_frames;

	enemy_frames = malloc(sizeof(t_animated_frames));
	if (!enemy_frames)
		return (NULL);
	enemy_frames->frames_count = ANIM_ENEMY_FRAMES_COUNT;
	enemy_frames->frames = malloc(sizeof(size_t *) * (enemy_frames->frames_count));
	if (!enemy_frames->frames)
	{
		safe_free(enemy_frames);
		return (NULL);
	}
	enemy_frames->frames[0] = settings_tex_get_texels(ANIM_ENEMY_IDLE_FRAME_0, mlx_handler);
	enemy_frames->frames[1] = settings_tex_get_texels(ANIM_ENEMY_IDLE_FRAME_1, mlx_handler);
	enemy_frames->frames[2] = settings_tex_get_texels(ANIM_ENEMY_IDLE_FRAME_2, mlx_handler);
	enemy_frames->frames[3] = settings_tex_get_texels(ANIM_ENEMY_IDLE_FRAME_3, mlx_handler);
	enemy_frames->frames[4] = settings_tex_get_texels(ANIM_ENEMY_IDLE_FRAME_4, mlx_handler);
	enemy_frames->frames[5] = settings_tex_get_texels(ANIM_ENEMY_IDLE_FRAME_5, mlx_handler);
	enemy_frames->frames[6] = settings_tex_get_texels(ANIM_ENEMY_IDLE_FRAME_6, mlx_handler);
	return (enemy_frames);
}

t_animated_frames	*settings_exit_frames_init(t_mlx_handler *mlx_handler)
{
	t_animated_frames	*exit_frames;

	exit_frames = malloc(sizeof(t_animated_frames));
	if (!exit_frames)
		return (NULL);
	exit_frames->frames_count = ANIM_EXIT_FRAMES_COUNT;
	exit_frames->frames = malloc(sizeof(size_t *) * (exit_frames->frames_count));
	if (!exit_frames->frames)
	{
		safe_free(exit_frames);
		return (NULL);
	}
	exit_frames->frames[0] = settings_tex_get_texels(ANIM_EXIT_IDLE_FRAME_0, mlx_handler);
	exit_frames->frames[1] = settings_tex_get_texels(ANIM_EXIT_IDLE_FRAME_1, mlx_handler);
	exit_frames->frames[2] = settings_tex_get_texels(ANIM_EXIT_IDLE_FRAME_2, mlx_handler);
	exit_frames->frames[3] = settings_tex_get_texels(ANIM_EXIT_IDLE_FRAME_3, mlx_handler);
	return (exit_frames);
}