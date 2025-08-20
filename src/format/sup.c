#include "sup.h"
#include "../debug/print.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

int recv_sup_config(int fd, struct sup_msg_config *msg)
{
	int recv_len;

	db_print_vb("| Client ID: %i\n", fd);

	/* Field: SUP version */
	recv_len = recv(fd, &msg->version, sizeof(int), 0);

	if (recv_len == -1)
	{
		perror("recv() in gfxenv_sup_configure_conn() failed");
		return -1;
	}

	if (recv_len == 0)
	{
		printf("Client disconnected mid-request.\b");
		return -1;
	}

	db_print_vb("| SUP Protocol Version: %i\n", msg->version);

	return 0;
}

int recv_sup_asset_load(int fd, struct sup_msg_asset_load *msg)
{
	char path_chr = 1;
	int path_len = 0;
	int recv_len;

	db_print_vb("| Client ID: %i\n", fd);

	/* Field: asset ID */
	recv_len = recv(fd, &msg->id, sizeof(int32_t), 0);

	if (recv_len == -1)
	{
		perror("recv() failed to retrieve asset ID");
		return -1;
	}

	if (recv_len == 0)
	{
		printf("Client disconnected mid-request.\b");
		return -1;
	}

	db_print_vb("| Asset ID: %i\n", msg->id);

	/* Field: asset path */
	msg->path = malloc(sizeof(char));

	/* Get path one character at a time, allocating space when needed.
	 * The string is null-terminated. */
	while (path_chr)
	{
		recv_len = recv(fd, &path_chr, sizeof(path_chr), 0);

		if (recv_len == -1)
		{
			perror("recv() failed to retrieve asset path");
			return -1;
		}

		if (recv_len == 0)
		{
			printf("Client disconnected mid-request.\b");
			return -1;
		}

		msg->path[path_len] = path_chr;

		++path_len;
		if (path_len & (path_len - 1) == 0)
		{
			msg->path = realloc(msg->path, path_len * 2);
	
			if (!msg->path)
			{
				perror("failed to reallocate string");
				return -1;
			}
		}
	}

	db_print_vb("| Asset path: ");
	db_print_vb("%s\n", msg->path);

	return 0;
}

int recv_sup_asset_show(int fd, struct sup_msg_asset_show *msg)
{
	int recv_len;

	db_print_vb("| Client ID: %i\n", fd);

	/* Field: asset ID */
	recv_len = recv(fd, &msg->id, sizeof(int32_t), 0);

    if (recv_len == -1)
	{
		perror("recv() failed to retrieve asset ID");
		return -1;
	}

    if (recv_len == 0)
    {
		printf("Client disconnected mid-request.\b");
        return -1;
    }

	db_print_vb("| Asset ID: %i\n", msg->id);

	return 0;
}

int recv_sup_asset_hide(int fd, struct sup_msg_asset_hide *msg)
{
	int recv_len;

	db_print_vb("| Client ID: %i\n", fd);

	/* Field: asset ID */
	recv_len = recv(fd, &msg->id, sizeof(int32_t), 0);

    if (recv_len == -1)
	{
		perror("recv() failed to retrieve asset ID");
		return -1;
	}

    if (recv_len == 0)
    {
		printf("Client disconnected mid-request.\b");
        return -1;
    }


	db_print_vb("| Asset ID: %i\n", msg->id);

	return 0;
}

int recv_sup_asset_tform(int fd, struct sup_msg_asset_tform *msg)
{
	int recv_len;

	db_print_vb("| Client ID: %i\n", fd);

	/* Field: asset ID */
	recv_len = recv(fd, &msg->id, sizeof(int32_t), 0);

    if (recv_len == -1)
	{
		perror("recv() failed to retrieve asset ID");
		return -1;
	}

    if (recv_len == 0)
    {
		printf("Client disconnected mid-request.\b");
        return -1;
    }

	db_print_vb("| Asset ID: %i\n", msg->id);

	/* Field: X location */
	recv_len = recv(fd, &msg->x_loc, sizeof(float), 0);

    if (recv_len == -1)
	{
		perror("recv() failed to retrieve X location");
		return -1;
	}

    if (recv_len == 0)
    {
		printf("Client disconnected mid-request.\b");
        return -1;
    }

	db_print_vb("| X location: %f\n", msg->x_loc);

	/* Field: Y location */
	recv_len = recv(fd, &msg->y_loc, sizeof(float), 0);

    if (recv_len == -1)
	{
		perror("recv() failed to retrieve Y location");
		return -1;
	}

    if (recv_len == 0)
    {
		printf("Client disconnected mid-request.\b");
        return -1;
    }


	db_print_vb("| Y location: %f\n", msg->y_loc);

	/* Field: Z location */
	recv_len = recv(fd, &msg->z_loc, sizeof(float), 0);

    if (recv_len == -1)
	{
		perror("recv() failed to retrieve Z location");
		return -1;
	}

    if (recv_len == 0)
    {
		printf("Client disconnected mid-request.\b");
        return -1;
    }


	db_print_vb("| Z location: %f\n", msg->z_loc);

	/* Field: X rotation */
	recv_len = recv(fd, &msg->x_rot, sizeof(float), 0);

    if (recv_len == -1)
	{
		perror("recv() failed to retrieve X rotation");
		return -1;
	}

    if (recv_len == 0)
    {
		printf("Client disconnected mid-request.\b");
        return -1;
    }


	db_print_vb("| X rotation: %f\n", msg->x_rot);

	/* Field: Y rotation */
	recv_len = recv(fd, &msg->y_rot, sizeof(float), 0);

    if (recv_len == -1)
	{
		perror("recv() failed to retrieve Y rotation");
		return -1;
	}

    if (recv_len == 0)
    {
		printf("Client disconnected mid-request.\b");
        return -1;
    }


	db_print_vb("| Y rotation: %f\n", msg->y_rot);

	/* Field: Z rotation */
	recv_len = recv(fd, &msg->z_rot, sizeof(float), 0);

    if (recv_len == -1)
	{
		perror("recv() failed to retrieve Z rotation");
		return -1;
	}

    if (recv_len == 0)
    {
		printf("Client disconnected mid-request.\b");
        return -1;
    }


	db_print_vb("| Z rotation: %f\n", msg->z_rot);

	/* Field: X scale */
	recv_len = recv(fd, &msg->x_scale, sizeof(float), 0);

    if (recv_len == -1)
	{
		perror("recv() failed to retrieve X scale");
		return -1;
	}

    if (recv_len == 0)
    {
		printf("Client disconnected mid-request.\b");
        return -1;
    }


	db_print_vb("| X scale: %f\n", msg->x_scale);

	/* Field: Y scale */
	recv_len = recv(fd, &msg->y_scale, sizeof(float), 0);

    if (recv_len == -1)
	{
		perror("recv() failed to retrieve Y scale");
		return -1;
	}

    if (recv_len == 0)
    {
		printf("Client disconnected mid-request.\b");
        return -1;
    }


	db_print_vb("| Y scale: %f\n", msg->y_scale);

	/* Field: Z scale */
	recv_len = recv(fd, &msg->z_scale, sizeof(float), 0);

    if (recv_len == -1)
	{
		perror("recv() failed to retrieve Z scale");
		return -1;
	}

    if (recv_len == 0)
    {
		printf("Client disconnected mid-request.\b");
        return -1;
    }


	db_print_vb("| Z scale: %f\n", msg->z_scale);

	return 0;
}

int recv_sup_asset_destroy(int fd, struct sup_msg_asset_destroy *msg)
{
	int recv_len;

	db_print_vb("| Client ID: %i\n", fd);

	/* Field: asset ID */
	recv_len = recv(fd, &msg->id, sizeof(int32_t), 0);

    if (recv_len == -1)
    {
        perror("recv() in recv_sup_asset_destroy() failed");
        return -1;
    }

    if (recv_len == 0)
    {
		printf("Client disconnected mid-request.\b");
        return -1;
    }

	db_print_vb("| Asset ID: %i\n", msg->id);

	return 0;
}

int recv_sup_action_create(int fd, struct sup_msg_action_create *msg)
{
	char path_chr = 1;
	int path_len = 0;
	int recv_len;

	db_print_vb("| Client ID: %i\n", fd);

    /* Field: Asset ID */
    recv_len = recv(fd, &msg->asset_id, sizeof(int32_t), 0);

    if (recv_len == -1)
    {
        perror("recv() in recv_sup_action_create() failed");
        return -1;
    }

    if (recv_len == 0)
    {
		printf("Client disconnected mid-request.\b");
        return -1;
    }

    printf("| Asset ID: %f\n", msg->asset_id);

    /* Field: action ID */
    recv_len = recv(fd, &msg->action_id, sizeof(int32_t), 0);

    if (recv_len == -1)
    {
        perror("recv() in recv_sup_action_create() failed");
        return -1;
    }

    if (recv_len == 0)
    {
		printf("Client disconnected mid-request.\b");
        return -1;
    }

    printf("| Action ID: %f\n", msg->action_id);

	/* Field: action name */
	msg->action_name = malloc(sizeof(char));

	/* Get path one character at a time, allocating space when needed.
	 * The string is null-terminated. */
	while (path_chr)
	{
		recv_len = recv(fd, &path_chr, sizeof(path_chr), 0);

		if (recv_len == -1)
		{
			perror("recv() in recv_sup_action_create() failed");
			return -1;
		}

		if (recv_len == 0)
		{
			printf("Client disconnected mid-request.\b");
			return -1;
		}

		msg->action_name[path_len] = path_chr;

		++path_len;
		if (path_len & (path_len - 1) == 0)
		{
			msg->action_name = realloc(msg->action_name, path_len * 2);
	
			if (!msg->action_name)
			{
				perror("failed to reallocate string");
				return -1;
			}
		}
	}

	db_print_vb("| Action name: ");
	db_print_vb("%s\n", msg->action_name);

	return 0;
}

int recv_sup_action_set_frame(int fd, struct sup_msg_action_set_frame *msg)
{
	int recv_len;
	
	db_print_vb("| Client ID: %i\n", fd);

    /* Field: action ID */
    recv_len = recv(fd, &msg->id, sizeof(int32_t), 0);

    if (recv_len == -1)
    {
        perror("recv() in recv_sup_action_set_frame() failed");
        return -1;
    }

    if (recv_len == 0)
    {
		printf("Client disconnected mid-request.\b");
        return -1;
    }

    printf("| Action ID: %f\n", msg->id);

    /* Field: action frame */
    recv_len = recv(fd, &msg->frame, sizeof(int32_t), 0);

    if (recv_len == -1)
    {
        perror("recv() in recv_sup_action_set_frame() failed");
        return -1;
    }

    if (recv_len == 0)
    {
		printf("Client disconnected mid-request.\b");
        return -1;
    }

    printf("| Frame: %f\n", msg->frame);

	return 0;
}

int recv_sup_action_set_weight(int fd, struct sup_msg_action_set_weight *msg)
{
	int recv_len;
	
	db_print_vb("| Client ID: %i\n", fd);

    /* Field: action ID */
    recv_len = recv(fd, &msg->id, sizeof(int32_t), 0);

    if (recv_len == -1)
    {
        perror("recv() in recv_sup_action_set_weight() failed");
        return -1;
    }

    if (recv_len == 0)
    {
		printf("Client disconnected mid-request.\b");
        return -1;
    }

    printf("| Action ID: %f\n", msg->id);

	/* Field: action weight */
	recv_len = recv(fd, &msg->weight, sizeof(float), 0);

    if (recv_len == -1)
    {
        perror("recv() in recv_sup_action_set_weight() failed");
        return -1;
    }

    if (recv_len == 0)
    {
		printf("Client disconnected mid-request.\b");
        return -1;
    }

	db_print_vb("| Action weight: %f\n", msg->weight);

	return 0;
}

int recv_sup_action_destroy(int fd, struct sup_msg_action_destroy *msg)
{
	int recv_len;
	
	db_print_vb("| Client ID: %i\n", fd);

    /* Field: action ID */
    recv_len = recv(fd, &msg->id, sizeof(int32_t), 0);

    if (recv_len == -1)
    {
        perror("recv() in recv_sup_action_destroy() failed");
        return -1;
    }

    if (recv_len == 0)
    {
		printf("Client disconnected mid-request.\b");
        return -1;
    }

    printf("| Action ID: %f\n", msg->id);

	return 0;
}

int recv_sup_view_tform(int fd, struct sup_msg_view_tform *msg)
{
	int recv_len;
	
	db_print_vb("| Client ID: %i\n", fd);

	/* Field: X location */
	recv_len = recv(fd, &msg->x_loc, sizeof(float), 0);

    if (recv_len == -1)
	{
		perror("recv() failed to retrieve X location");
		return -1;
	}

    if (recv_len == 0)
    {
		printf("Client disconnected mid-request.\b");
        return -1;
    }


	db_print_vb("| X location: %f\n", msg->x_loc);

	/* Field: Y location */
	recv_len = recv(fd, &msg->y_loc, sizeof(float), 0);

    if (recv_len == -1)
	{
		perror("recv() failed to retrieve Y location");
		return -1;
	}

    if (recv_len == 0)
    {
		printf("Client disconnected mid-request.\b");
        return -1;
    }


	db_print_vb("| Y location: %f\n", msg->y_loc);

	/* Field: Z location */
	recv_len = recv(fd, &msg->z_loc, sizeof(float), 0);

    if (recv_len == -1)
	{
		perror("recv() failed to retrieve Z location");
		return -1;
	}

    if (recv_len == 0)
    {
		printf("Client disconnected mid-request.\b");
        return -1;
    }


	db_print_vb("| Z location: %f\n", msg->z_loc);

	/* Field: X rotation */
	recv_len = recv(fd, &msg->x_rot, sizeof(float), 0);

    if (recv_len == -1)
	{
		perror("recv() failed to retrieve X rotation");
		return -1;
	}

    if (recv_len == 0)
    {
		printf("Client disconnected mid-request.\b");
        return -1;
    }


	db_print_vb("| X rotation: %f\n", msg->x_rot);

	/* Field: Y rotation */
	recv_len = recv(fd, &msg->y_rot, sizeof(float), 0);

    if (recv_len == -1)
	{
		perror("recv() failed to retrieve Y rotation");
		return -1;
	}

    if (recv_len == 0)
    {
		printf("Client disconnected mid-request.\b");
        return -1;
    }


	db_print_vb("| Y rotation: %f\n", msg->y_rot);

	/* Field: Z rotation */
	recv_len = recv(fd, &msg->z_rot, sizeof(float), 0);

    if (recv_len == -1)
	{
		perror("recv() failed to retrieve Z rotation");
		return -1;
	}

    if (recv_len == 0)
    {
		printf("Client disconnected mid-request.\b");
        return -1;
    }


	db_print_vb("| Z rotation: %f\n", msg->z_rot);



	return 0;
}

int recv_sup_view_set_fov(int fd, struct sup_msg_view_set_fov *msg)
{
	int recv_len;
	
	db_print_vb("| Client ID: %i\n", fd);

	/* Field: FOV */
	recv_len = recv(fd, &msg->fov, sizeof(float), 0);

    if (recv_len == -1)
    {
        perror("recv() in recv_sup_view_set_fov() failed");
        return -1;
    }

	if (recv_len == 0)
    {
		printf("Client disconnected mid-request.\b");
        return -1;
    }


	db_print_vb("| Field of view: %f\n", msg->fov);

	return 0;
}


