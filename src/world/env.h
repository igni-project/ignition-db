#ifndef WORLD_ENV_H
#define WORLD_ENV_H 1

struct vertex
{
	float pos[3];
	float normal[3];
	float texCoord[2];
};

struct gfxenv_scene
{
	int fd;
	int sup_version;
};

struct gfxenv
{
	struct gfxenv_scene *scenes;
	unsigned int scene_count;
	unsigned int scene_limit;
};

int create_gfxenv(struct gfxenv *gfxenv);

void destroy_gfxenv(struct gfxenv gfxenv);

int gfxenv_new_client(struct gfxenv *gfxenv, int fd);

int gfxenv_exec(struct gfxenv *gfxenv, unsigned int scene);

int gfxenv_del_scene(struct gfxenv *gfxenv, unsigned int scene);

int gfxenv_sup_configure_conn(struct gfxenv *gfxenv, unsigned int scene);

int gfxenv_sup_asset_load(struct gfxenv *gfxenv, unsigned int scene);
int gfxenv_sup_asset_show(struct gfxenv *gfxenv, unsigned int scene);
int gfxenv_sup_asset_hide(struct gfxenv *gfxenv, unsigned int scene);
int gfxenv_sup_asset_tform(struct gfxenv *gfxenv, unsigned int scene);
int gfxenv_sup_asset_destroy(struct gfxenv *gfxenv, unsigned int scene);

int gfxenv_sup_action_create(struct gfxenv *gfxenv, unsigned int scene);
int gfxenv_sup_action_set_frame(struct gfxenv *gfxenv, unsigned int scene);
int gfxenv_sup_action_set_weight(struct gfxenv *gfxenv, unsigned int scene);
int gfxenv_sup_action_destroy(struct gfxenv *gfxenv, unsigned int scene);

int gfxenv_sup_view_tform(struct gfxenv *gfxenv, unsigned int scene);
int gfxenv_sup_view_set_fov(struct gfxenv *gfxenv, unsigned int scene);

#endif

