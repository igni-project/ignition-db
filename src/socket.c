#include "socket.h"

#include <libsup/sup.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/time.h>

#define MSG_BUFFER_SIZE 1024

/* buffer for recv() to put received data in */
char buffer[MSG_BUFFER_SIZE];

int create_sock_arr(sock_arr* arr)
{
	arr->count = 0;
	arr->max = 1;
	arr->fds = malloc(sizeof(int));
	if (!arr->fds)
	{
		perror("Failed to allocate space for socket descriptor array");
		return -1;
	}

	return 0;
}

int add_new_client(sock_arr *arr, int fd)
{
	/* The reallocated address is audited before use */
	int *new_addr;

	if (arr->count >= arr->max)
	{
		arr->max *= 2;
		new_addr = realloc(arr->fds, arr->max);

		if (!new_addr)
		{
			perror("add_new_client() failed: could not allocate memory");
			return -1;
		}

		arr->fds = new_addr;
	}

	arr->fds[arr->count] = fd;

	++arr->count;

	printf("\033[1mNew client\033[0m\n");
	printf("| Client ID: %i\n", fd);

	return 0;
}

int remove_client(sock_arr *arr, int idx)
{
	/* The reallocated address is audited before use */
	int *new_addr;

	--arr->count;

	memcpy(
		&arr->fds[idx],
		&arr->fds[idx + 1],
		(arr->count - idx) * sizeof(int)
	);

	if (arr->count < arr->max / 2)
	{
		arr->max /= 2;
		new_addr = realloc(arr->fds, arr->max);

		if (!new_addr)
		{
			perror("remove_client() failed: could not allocate memory");
			return -1;
		}

		arr->fds = new_addr;
	}

	printf("\033[1mClient disconnect\033[0m\n");
	printf("| Client ID: %i\n", arr->fds[idx]);

	return 0;
}

int proc_msg(sock_arr *arr, int idx)
{
	uint8_t req_code;
	int recv_status;

	/* Socket that received message to process */
	int client_fd = arr->fds[idx];

	recv_status = recv(client_fd, &req_code, sizeof(req_code), 0);

	if (recv_status == -1)
	{
		perror("An error occurred while receiving data");
		remove_client(arr, idx);
		return 0;
	}

	/* A message size of 0 means client has disconnected */
	if (!recv_status)
	{
		remove_client(arr, idx);
		return 0;
	}

	/* Handle request code */
	switch (req_code)
	{
	case SUP_REQ_CODE_CONFIGURE_CONNECTION:
		if (proc_config(client_fd))
		{
			remove_client(arr, idx);
		}
		break;

	case SUP_REQ_CODE_ASSET_LOAD:
		if (proc_asset_load(client_fd))
		{
			remove_client(arr, idx);
		}

		break;

	case SUP_REQ_CODE_ASSET_SHOW:
		if (proc_asset_show(client_fd))
		{
			remove_client(arr, idx);
		}

		break;

	case SUP_REQ_CODE_ASSET_HIDE:
		if (proc_asset_hide(client_fd))
		{
			remove_client(arr, idx);
		}

		break;

	case SUP_REQ_CODE_ASSET_TRANSFORM:
		if (proc_asset_tform(client_fd))
		{
			remove_client(arr, idx);
		}

		break;

	case SUP_REQ_CODE_ASSET_DESTROY:
		if (proc_asset_destroy(client_fd))
		{
			remove_client(arr, idx);
		}

		break;

	case SUP_REQ_CODE_ACTION_CREATE_INSTANCE:
		if (proc_action_load(client_fd))
		{
			remove_client(arr, idx);
		}

		break;

	case SUP_REQ_CODE_ACTION_SET_FRAME:
		if (proc_action_set_frame(client_fd))
		{
			remove_client(arr, idx);
		}

		break;

	case SUP_REQ_CODE_ACTION_SET_WEIGHT:
		if (proc_action_set_weight(client_fd))
		{
			remove_client(arr, idx);
		}

		break;

	case SUP_REQ_CODE_ACTION_DESTROY_INSTANCE:
		if (proc_action_destroy(client_fd))
		{
			remove_client(arr, idx);
		}

		break;

	case SUP_REQ_CODE_VIEWPOINT_TRANSFORM:
		if (proc_viewpoint_tform(client_fd))
		{
			remove_client(arr, idx);
		}

		break;

	case SUP_REQ_CODE_VIEWPOINT_SET_FIELD_OF_VIEW:
		if (proc_viewpoint_set_fov(client_fd))
		{
			remove_client(arr, idx);
		}

		break;

	default:
		printf("Unknown request code.\n");
		remove_client(arr, idx);
	}

	return 0;
}

int proc_config(int fd)
{
	int recv_status;
	int32_t recv_int32;

	printf("\033[1mNew request\033[0m (0: configure connection)\n");
	printf("| Client ID: %i\n", fd);

	/* Field: Version */
	recv_status = recv(fd, &recv_int32, sizeof(recv_int32), 0);

	if (recv_status == -1)
	{
		perror("recv() in proc_config() failed");
		return -1;
	}

	if (recv_status == 0)
	{
		return -1;
	}

	printf("| SUP Protocol Version: %i\n", recv_int32);

	return 0;
}

int proc_asset_load(int fd)
{
	int recv_status;
	int pathlen;
	int32_t recv_int32;
	char path_chr = 1;

	printf("\033[1mNew request\033[0m (1: load asset)\n");

	printf("| Client ID: %i\n", fd);

	/* Field: asset ID */
	recv_status = recv(fd, &recv_int32, sizeof(recv_int32), 0);

	if (recv_status == -1)
	{
		perror("recv() in proc_asset_load() failed");
		return -1;
	}

	if (recv_status == 0)
	{
		return -1;
	}

	printf("| Asset ID: %i\n", recv_int32);

	/* Print out recieved string character by character */

	printf("| Asset path: ");
	while (path_chr)
	{
		recv_status = recv(fd, &path_chr, sizeof(path_chr), 0);

		if (recv_status == -1)
		{
			perror("recv() in proc_asset_load() failed");
			return -1;
		}

		if (recv_status == 0)
		{
			return -1;
		}

		printf("%c", path_chr);
	}

	printf("\n");

	return 0;
}

int proc_asset_show(int fd)
{
	int recv_status;
	int32_t recv_int32;

	printf("\033[1mNew request\033[0m (2: show asset)\n");
	printf("| Client ID: %i\n", fd);

	/* Field: asset ID */
	recv_status = recv(fd, &recv_int32, sizeof(recv_int32), 0);

	if (recv_status == -1)
	{
		perror("recv() in proc_asset_show() failed");
		return -1;
	}

	if (recv_status == 0)
	{
		return -1;
	}

	printf("| Asset ID: %i\n", recv_int32);

	return 0;
}


int proc_asset_hide(int fd)
{
	int recv_status;
	int32_t recv_int32;

	printf("\033[1mNew request\033[0m (3: hide asset)\n");
	printf("| Client ID: %i\n", fd);

	/* Field: asset ID */
	recv_status = recv(fd, &recv_int32, sizeof(recv_int32), 0);

	if (recv_status == -1)
	{
		perror("recv() in proc_asset_hide() failed");
		return -1;
	}

	if (recv_status == 0)
	{
		return -1;
	}

	printf("| Asset ID: %i\n", recv_int32);

	return 0;
}

int proc_asset_tform(int fd)
{
	int recv_status;
	int32_t recv_int32;
	float recv_float; /* reused for each field for the time being */

	printf("\033[1mNew request\033[0m (4: transform asset)\n");
	printf("| Client ID: %i\n", fd);

	/* Field: Asset ID */
	recv_status = recv(fd, &recv_int32, sizeof(recv_int32), 0);

	if (recv_status == -1)
	{
		perror("recv() in proc_asset_tform() failed");
		return -1;
	}

	if (recv_status == 0)
	{
		return -1;
	}

	printf("| Asset ID: %f\n", recv_int32);

	/* Field: X location */
	recv_status = recv(fd, &recv_float, sizeof(recv_float), 0);

	if (recv_status == -1)
	{
		perror("recv() in proc_asset_tform() failed");
		return -1;
	}

	if (recv_status == 0)
	{
		return -1;
	}

	printf("| X location: %f\n", recv_float);

	/* Field: Y location */
	recv_status = recv(fd, &recv_float, sizeof(recv_float), 0);

	if (recv_status == -1)
	{
		perror("recv() in proc_asset_tform() failed");
		return -1;
	}

	if (recv_status == 0)
	{
		return -1;
	}

	printf("| Y location: %f\n", recv_float);

	/* Field: Z location */
	recv_status = recv(fd, &recv_float, sizeof(recv_float), 0);

	if (recv_status == -1)
	{
		perror("recv() in proc_asset_tform() failed");
		return -1;
	}

	if (recv_status == 0)
	{
		return -1;
	}

	printf("| Z location: %f\n", recv_float);

	/* Field: X rotation */
	recv_status = recv(fd, &recv_float, sizeof(recv_float), 0);

	if (recv_status == -1)
	{
		perror("recv() in proc_asset_tform() failed");
		return -1;
	}

	if (recv_status == 0)
	{
		return -1;
	}

	printf("| X rotation: %f\n", recv_float);

	/* Field: Y rotation */
	recv_status = recv(fd, &recv_float, sizeof(recv_float), 0);

	if (recv_status == -1)
	{
		perror("recv() in proc_asset_tform() failed");
		return -1;
	}

	if (recv_status == 0)
	{
		return -1;
	}

	printf("| Y rotation: %f\n", recv_float);

	/* Field: Z rotation */
	recv_status = recv(fd, &recv_float, sizeof(recv_float), 0);

	if (recv_status == -1)
	{
		perror("recv() in proc_asset_tform() failed");
		return -1;
	}

	if (recv_status == 0)
	{
		return -1;
	}

	printf("| Z rotation: %f\n", recv_float);

	/* Field: X scale */
	recv_status = recv(fd, &recv_float, sizeof(recv_float), 0);

	if (recv_status == -1)
	{
		perror("recv() in proc_asset_tform() failed");
		return -1;
	}

	if (recv_status == 0)
	{
		return -1;
	}

	printf("| X scale: %f\n", recv_float);

	/* Field: Y scale */
	recv_status = recv(fd, &recv_float, sizeof(recv_float), 0);

	if (recv_status == -1)
	{
		perror("recv() in proc_asset_tform() failed");
		return -1;
	}

	if (recv_status == 0)
	{
		return -1;
	}

	printf("| Y scale: %f\n", recv_float);

	/* Field: Z scale */
	recv_status = recv(fd, &recv_float, sizeof(recv_float), 0);

	if (recv_status == -1)
	{
		perror("recv() in proc_asset_tform() failed");
		return -1;
	}

	if (recv_status == 0)
	{
		return -1;
	}

	printf("| Z scale: %f\n", recv_float);

	return 0;
}

int proc_asset_destroy(int fd)
{
	int recv_status;
	int32_t recv_int32;

	printf("\033[1mNew request\033[0m (5: destroy asset)\n");
	printf("| Client ID: %i\n", fd);

	/* Field: asset ID */
	recv_status = recv(fd, &recv_int32, sizeof(recv_int32), 0);

	if (recv_status == -1)
	{
		perror("recv() in proc_asset_destroy() failed");
		return -1;
	}

	if (recv_status == 0)
	{
		return -1;
	}

	printf("| Asset ID: %i\n", recv_int32);

	return 0;
}


int proc_action_load(int fd)
{
	int recv_status;
	int pathlen;
	int32_t recv_int32;
	char path_chr = 1;

	printf("\033[1mNew request\033[0m (6: create action instance)\n");
	printf("| Client ID: %i\n", fd);

	/* Field: Asset ID */
	recv_status = recv(fd, &recv_int32, sizeof(recv_int32), 0);

	if (recv_status == -1)
	{
		perror("recv() in proc_action_load() failed");
		return -1;
	}

	if (recv_status == 0)
	{
		return -1;
	}

	printf("| Asset ID: %f\n", recv_int32);

	/* Field: action ID */
	recv_status = recv(fd, &recv_int32, sizeof(recv_int32), 0);

	if (recv_status == -1)
	{
		perror("recv() in proc_action_load() failed");
		return -1;
	}

	if (recv_status == 0)
	{
		return -1;
	}

	printf("| Action ID: %i\n", recv_int32);

	/* Print out recieved action name character by character */

	printf("| Action name: ");
	while (path_chr)
	{
		recv_status = recv(fd, &path_chr, sizeof(path_chr), 0);

		if (recv_status == 0)
		{
			return -1;
		}

		if (recv_status == -1)
		{
			perror("recv() in proc_action_load() failed");
			return -1;
		}

		printf("%c", path_chr);
	}

	printf("\n");


	return 0;
}


int proc_action_set_frame(int fd)
{
	int recv_status;
	int32_t recv_int32;

	printf("\033[1mNew request\033[0m (7: set action frame)\n");
	printf("| Client ID: %i\n", fd);

	/* Field: action ID */
	recv_status = recv(fd, &recv_int32, sizeof(recv_int32), 0);

	if (recv_status == 0)
	{
		return -1;
	}

	if (recv_status == -1)
	{
		perror("recv() in proc_action_set_frame() failed");
		return -1;
	}

	printf("| Action ID: %i\n", recv_int32);

	/* Field: action frame */
	recv_status = recv(fd, &recv_int32, sizeof(recv_int32), 0);

	if (recv_status == 0)
	{
		return -1;
	}

	if (recv_status == -1)
	{
		perror("recv() in proc_action_set_frame() failed");
		return -1;
	}

	printf("| Frame: %i\n", recv_int32);


	return 0;
}


int proc_action_set_weight(int fd)
{
	int recv_status;
	int32_t recv_int32;
	float recv_float;

	printf("\033[1mNew request\033[0m (8: set action weight)\n");
	printf("| Client ID: %i\n", fd);

	/* Field: action ID */
	recv_status = recv(fd, &recv_int32, sizeof(recv_int32), 0);

	if (recv_status == 0)
	{
		return -1;
	}

	if (recv_status == -1)
	{
		perror("recv() in proc_action_set_weight() failed");
		return -1;
	}

	printf("| Action ID: %i\n", recv_int32);

	/* Field: action weight */
	recv_status = recv(fd, &recv_float, sizeof(recv_float), 0);

	if (recv_status == 0)
	{
		return -1;
	}

	if (recv_status == -1)
	{
		perror("recv() in proc_action_set_weight() failed");
		return -1;
	}

	printf("| Weight: %f\n", recv_float);

	return 0;
}


int proc_action_destroy(int fd)
{
	int recv_status;
	int32_t recv_int32;

	printf("\033[1mNew request\033[0m (9: destroy action instance)\n");
	printf("| Client ID: %i\n", fd);

	/* Field: action ID */
	recv_status = recv(fd, &recv_int32, sizeof(recv_int32), 0);

	if (recv_status == 0)
	{
		return -1;
	}

	if (recv_status == -1)
	{
		perror("recv() in proc_action_destroy() failed");
		return -1;
	}

	printf("| Action ID: %i\n", recv_int32);

	return 0;
}



int proc_viewpoint_tform(int fd)
{
	int recv_status;
	float recv_float;

	printf("\033[1mNew request\033[0m (10: transform viewpoint)\n");
	printf("| Client ID: %i\n", fd);

	/* Field: X location */
	recv_status = recv(fd, &recv_float, sizeof(recv_float), 0);

	if (recv_status == 0)
	{
		return -1;
	}

	if (recv_status == -1)
	{
		perror("recv() in proc_viewpoint_tform() failed");
		return -1;
	}

	printf("| X location: %f\n", recv_float);

	/* Field: Y location */
	recv_status = recv(fd, &recv_float, sizeof(recv_float), 0);

	if (recv_status == 0)
	{
		return -1;
	}

	if (recv_status == -1)
	{
		perror("recv() in proc_viewpoint_tform() failed");
		return -1;
	}

	printf("| Y location: %f\n", recv_float);

	/* Field: Z location */
	recv_status = recv(fd, &recv_float, sizeof(recv_float), 0);

	if (recv_status == 0)
	{
		return -1;
	}

	if (recv_status == -1)
	{
		perror("recv() in proc_viewpoint_tform() failed");
		return -1;
	}

	printf("| Z location: %f\n", recv_float);

	/* Field: X rotation */
	recv_status = recv(fd, &recv_float, sizeof(recv_float), 0);

	if (recv_status == 0)
	{
		return -1;
	}

	if (recv_status == -1)
	{
		perror("recv() in proc_viewpoint_tform() failed");
		return -1;
	}

	printf("| X rotation: %f\n", recv_float);

	/* Field: Y rotation */
	recv_status = recv(fd, &recv_float, sizeof(recv_float), 0);

	if (recv_status == 0)
	{
		return -1;
	}

	if (recv_status == -1)
	{
		perror("recv() in proc_viewpoint_tform() failed");
		return -1;
	}

	printf("| Y rotation: %f\n", recv_float);

	/* Field: Z rotation */
	recv_status = recv(fd, &recv_float, sizeof(recv_float), 0);

	if (recv_status == 0)
	{
		return -1;
	}

	if (recv_status == -1)
	{
		perror("recv() in proc_viewpoint_tform() failed");
		return -1;
	}

	printf("| Z rotation: %f\n", recv_float);

	return 0;
}


int proc_viewpoint_set_fov(int fd)
{
	int recv_status;
	float recv_float;

	printf("\033[1mNew request\033[0m (11: set viewpoint field of view)\n");
	printf("| Client ID: %i\n", fd);

	/* Field: FOV */
	recv_status = recv(fd, &recv_float, sizeof(recv_float), 0);

	if (recv_status == 0)
	{
		return -1;
	}

	if (recv_status == -1)
	{
		perror("recv() in proc_viewpoint_set_fov() failed");
		return -1;
	}

	printf("| FOV: %f\n", recv_float);

	return 0;
}



