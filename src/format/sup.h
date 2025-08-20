#ifndef FORMAT_SUP_H
#define FORMAT_SUP_H 1

#include <stdint.h>

struct sup_msg_config
{
	int version;
};

struct sup_msg_asset_load
{
	int32_t id;
	char *path;
};

struct sup_msg_asset_show
{
	int32_t id;
};

struct sup_msg_asset_hide
{
	int32_t id;
};

struct sup_msg_asset_tform
{
	int32_t id;
	float x_loc, y_loc, z_loc;
	float x_rot, y_rot, z_rot;
	float x_scale, y_scale, z_scale;
};

struct sup_msg_asset_destroy
{
	int32_t id;
};

struct sup_msg_action_create
{
	int32_t asset_id; 
	int32_t action_id; 
	char *action_name;
};

struct sup_msg_action_set_frame
{
	int32_t id;
	int32_t frame;
};

struct sup_msg_action_set_weight
{
	int32_t id;
	float weight;
};

struct sup_msg_action_destroy
{
	int32_t id;
};

struct sup_msg_view_tform
{
	float x_loc, y_loc, z_loc;
	float x_rot, y_rot, z_rot;
};

struct sup_msg_view_set_fov
{
	float fov;
};

int recv_sup_config(int fd, struct sup_msg_config *msg);

int recv_sup_asset_load(int fd, struct sup_msg_asset_load *msg);
int recv_sup_asset_show(int fd, struct sup_msg_asset_show *msg);
int recv_sup_asset_hide(int fd, struct sup_msg_asset_hide *msg);
int recv_sup_asset_tform(int fd, struct sup_msg_asset_tform *msg);
int recv_sup_asset_destroy(int fd, struct sup_msg_asset_destroy *msg);

int recv_sup_action_create(int fd, struct sup_msg_action_create *msg);
int recv_sup_action_set_frame(int fd, struct sup_msg_action_set_frame *msg);
int recv_sup_action_set_weight(int fd, struct sup_msg_action_set_weight *msg);
int recv_sup_action_destroy(int fd, struct sup_msg_action_destroy *msg);

int recv_sup_view_tform(int fd, struct sup_msg_view_tform *msg);
int recv_sup_view_set_fov(int fd, struct sup_msg_view_set_fov *msg);

#endif

