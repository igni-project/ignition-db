#include "gfxenv.h"
#include "../debug/print.h"
#include "../format/sup.h"

#include <libsup/sup.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

int create_gfxenv(struct gfxenv *gfxenv)
{
	gfxenv->scenes = malloc(sizeof(int));
	gfxenv->scene_count = 0;
	gfxenv->scene_limit = 1;

	if (!gfxenv->scenes)
	{
		printf("Could not allocate memory for scenes!\n");
		return -1;
	}

	return 0;
}

void destroy_gfxenv(struct gfxenv gfxenv)
{
	free(gfxenv.scenes);
}

int gfxenv_new_client(struct gfxenv *gfxenv, int fd)
{
	/* The reallocated address is audited before use */
	void *new_addr;
	
	if (gfxenv->scene_count >= gfxenv->scene_limit)
	{
		gfxenv->scene_limit *= 2;
	
		/* Reallocate socket descriptor array */
		new_addr = realloc(gfxenv->scenes, gfxenv->scene_limit);
		if (!new_addr)
		{
			return -1;
			perror("add_new_client() failed: could not allocate memory");
		}

		gfxenv->scenes = new_addr;

	}

	gfxenv->scenes[gfxenv->scene_count].fd = fd;

	++gfxenv->scene_count;

	db_print_vb("\033[1mNew client\033[0m\n");
	db_print_vb("| Client ID: %i\n", fd);

	return 0;
}

int gfxenv_del_scene(struct gfxenv *gfxenv, unsigned int scene)
{
	void *new_addr;

	if (scene > gfxenv->scene_count)
	{
		printf("Scene index out of range! (%i)\n", scene);
		return -1;
	}

	close(gfxenv->scenes[scene].fd);

	db_print_vb("| Client ID: %i\n", gfxenv->scenes[scene].fd);

	--gfxenv->scene_count;

	memcpy(
		&gfxenv->scenes[scene],
		&gfxenv->scenes[scene + 1],
		(gfxenv->scene_count - scene) * sizeof(struct gfxenv_scene)
	);

	if (gfxenv->scene_count < gfxenv->scene_limit / 2)
	{
		gfxenv->scene_limit /= 2;

		/* Reallocate socket descriptor array */
		new_addr = realloc(gfxenv->scenes, gfxenv->scene_limit);
		if (!new_addr)
		{
			perror("realloc() in gfxenv_del_scene_gl() failed:");
			return -1;
		}
		gfxenv->scenes = (struct gfxenv_scene *)new_addr;
	}

	return 0;
}

int gfxenv_exec(struct gfxenv *gfxenv, unsigned int scene)
{
	/* The first field in every request: the opcode. */
	uint8_t opcode;

	/* Length of received data */
	int recv_len;
	
	/* Socket descriptor */
	int fd;

	if (scene > gfxenv->scene_count)
	{
		printf("Scene index out of range!\n");

		db_print_vb("\033[1mClient disconnect\033[0m\n");
		gfxenv_del_scene(gfxenv, scene);
		
		return -1;
	}

	fd = gfxenv->scenes[scene].fd;

	recv_len = recv(fd, &opcode, sizeof(opcode), 0);

	/* A recv() length of -1 signals an error. */
	if (recv_len == -1)
	{
		perror("An error occurred while receiving data");

		db_print_vb("\033[1mClient disconnect\033[0m\n");
		gfxenv_del_scene(gfxenv, scene);

		return -1;
	}

	/* A length of 0 means the socket disconnected with no error. */
	if (!recv_len)
	{
		db_print_vb("\033[1mClient disconnect\033[0m\n");
		gfxenv_del_scene(gfxenv, scene);

		return 0;
	}

	switch (opcode)
	{
	case SUP_REQ_CODE_CONFIGURE_CONNECTION:
		return gfxenv_sup_configure_conn(gfxenv, scene);

	case SUP_REQ_CODE_ASSET_LOAD:
		return gfxenv_sup_asset_load(gfxenv, scene);

	case SUP_REQ_CODE_ASSET_SHOW:
		return gfxenv_sup_asset_show(gfxenv, scene);

	case SUP_REQ_CODE_ASSET_HIDE:
		return gfxenv_sup_asset_hide(gfxenv, scene);

	case SUP_REQ_CODE_ASSET_TRANSFORM:
		return gfxenv_sup_asset_tform(gfxenv, scene);

	case SUP_REQ_CODE_ASSET_DESTROY:
		return gfxenv_sup_asset_destroy(gfxenv, scene);

	case SUP_REQ_CODE_ACTION_CREATE_INSTANCE:
		return gfxenv_sup_action_create(gfxenv, scene);

	case SUP_REQ_CODE_ACTION_SET_FRAME:
		return gfxenv_sup_action_set_frame(gfxenv, scene);

	case SUP_REQ_CODE_ACTION_SET_WEIGHT:
		return gfxenv_sup_action_set_weight(gfxenv, scene);

	case SUP_REQ_CODE_ACTION_DESTROY_INSTANCE:
		return gfxenv_sup_action_destroy(gfxenv, scene);

	case SUP_REQ_CODE_VIEWPOINT_TRANSFORM:
		return gfxenv_sup_view_tform(gfxenv, scene);

	case SUP_REQ_CODE_VIEWPOINT_SET_FIELD_OF_VIEW:
		return gfxenv_sup_view_set_fov(gfxenv, scene);

	default:
		printf("Unknown opcode: %i\n", opcode);
		gfxenv_del_scene(gfxenv, scene);
		return -1;
	}

	return 0;
}

int gfxenv_sup_configure_conn(struct gfxenv *gfxenv, unsigned int scene)
{
	struct sup_msg_config msg = {0};

	db_print_vb("\033[1mNew request\033[0m (0: configure connection)\n");

	if (scene > gfxenv->scene_count)
	{
		printf("Scene index out of range!\n");
		return -1;
	}

	if (recv_sup_config(gfxenv->scenes[scene].fd, &msg) == -1)
	{
		return -1;
	}

	gfxenv->scenes[scene].sup_version = msg.version;

	return 0;
}

int gfxenv_sup_asset_load(struct gfxenv *gfxenv, unsigned int scene)
{
	struct sup_msg_asset_load msg;

	db_print_vb("\033[1mNew request\033[0m (1: load asset)\n");

	if (scene > gfxenv->scene_count)
	{
		printf("Scene index out of range!\n");
		return -1;
	}

	if (recv_sup_asset_load(gfxenv->scenes[scene].fd, &msg) == -1)
	{
		return -1;
	}

	return 0;
}

int gfxenv_sup_asset_show(struct gfxenv *gfxenv, unsigned int scene)
{
	struct sup_msg_asset_show msg;

	db_print_vb("\033[1mNew request\033[0m (2: show asset)\n");

	if (scene > gfxenv->scene_count)
	{
		printf("Scene index out of range!\n");
		return -1;
	}

	if (recv_sup_asset_show(gfxenv->scenes[scene].fd, &msg) == -1)
	{
		return -1;
	}

	return 0;
}

int gfxenv_sup_asset_hide(struct gfxenv *gfxenv, unsigned int scene)
{
	struct sup_msg_asset_hide msg;

	db_print_vb("\033[1mNew request\033[0m (3: hide asset)\n");

	if (scene > gfxenv->scene_count)
	{
		printf("Scene index out of range!\n");
		return -1;
	}

	if (recv_sup_asset_hide(gfxenv->scenes[scene].fd, &msg) == -1)
	{
		return -1;
	}

	return 0;
}

int gfxenv_sup_asset_tform(struct gfxenv *gfxenv, unsigned int scene)
{
	struct sup_msg_asset_tform msg;

	db_print_vb("\033[1mNew request\033[0m (4: transform asset)\n");

	if (scene > gfxenv->scene_count)
	{
		printf("Scene index out of range!\n");
		return -1;
	}

	if (recv_sup_asset_tform(gfxenv->scenes[scene].fd, &msg) == -1)
	{
		return -1;
	}

	return 0;
}

int gfxenv_sup_asset_destroy(struct gfxenv *gfxenv, unsigned int scene)
{
	struct sup_msg_asset_destroy msg;

	db_print_vb("\033[1mNew request\033[0m (5: destroy asset)\n");

	if (scene > gfxenv->scene_count)
	{
		printf("Scene index out of range!\n");
		return -1;
	}

	if (recv_sup_asset_destroy(gfxenv->scenes[scene].fd, &msg) == -1)
	{
		return -1;
	}

	return 0;
}

int gfxenv_sup_action_create(struct gfxenv *gfxenv, unsigned int scene)
{
	struct sup_msg_action_create msg;

	db_print_vb("\033[1mNew request\033[0m (6: create action instance)\n");

	if (scene > gfxenv->scene_count)
	{
		printf("Scene index out of range!\n");
		return -1;
	}

	if (recv_sup_action_create(gfxenv->scenes[scene].fd, &msg) == -1)
	{
		return -1;
	}

	return 0;
}

int gfxenv_sup_action_set_frame(struct gfxenv *gfxenv, unsigned int scene)
{
	struct sup_msg_action_set_frame msg;
	 
	db_print_vb("\033[1mNew request\033[0m (7: set action frame)\n");

	if (scene > gfxenv->scene_count)
	{
		printf("Scene index out of range!\n");
		return -1;
	}

	if (recv_sup_action_set_frame(gfxenv->scenes[scene].fd, &msg) == -1)
	{
		return -1;
	}

	return 0;
}

int gfxenv_sup_action_set_weight(struct gfxenv *gfxenv, unsigned int scene)
{
	struct sup_msg_action_set_weight msg;

	db_print_vb("\033[1mNew request\033[0m (8: set action weight)\n");

	if (scene > gfxenv->scene_count)
	{
		printf("Scene index out of range!\n");
		return -1;
	}

	if (recv_sup_action_set_weight(gfxenv->scenes[scene].fd, &msg) == -1)
	{
		return -1;
	}

	return 0;
}

int gfxenv_sup_action_destroy(struct gfxenv *gfxenv, unsigned int scene)
{
	struct sup_msg_action_destroy msg;

	db_print_vb("\033[1mNew request\033[0m (9: destroy action instance)\n");

	if (scene > gfxenv->scene_count)
	{
		printf("Scene index out of range!\n");
		return -1;
	}

	if (recv_sup_action_destroy(gfxenv->scenes[scene].fd, &msg) == -1)
	{
		return -1;
	}

	return 0;
}

int gfxenv_sup_view_tform(struct gfxenv *gfxenv, unsigned int scene)
{
	struct sup_msg_view_tform msg;

	db_print_vb("\033[1mNew request\033[0m (10: transform viewpoint)\n");

	if (scene > gfxenv->scene_count)
	{
		printf("Scene index out of range!\n");
		return -1;
	}

	if (recv_sup_view_tform(gfxenv->scenes[scene].fd, &msg) == -1)
	{
		return -1;
	}

	return 0;
}

int gfxenv_sup_view_set_fov(struct gfxenv *gfxenv, unsigned int scene)
{
	struct sup_msg_view_set_fov msg;

	db_print_vb("\033[1mNew request\033[0m (11: set viewpoint field of view)\n");

	if (scene > gfxenv->scene_count)
	{
		printf("Scene index out of range!\n");
		return -1;
	}

	if (recv_sup_view_set_fov(gfxenv->scenes[scene].fd, &msg) == -1)
	{
		return -1;
	}

	return 0;
}


