#ifndef UDSCAST_SOCKET_H
#define UDSCAST_SOCKET_H 1

#include <stddef.h>

typedef struct sock_arr_t
{
	/* Socket descriptors */
	int *fds;
	/* Socket count */
	int count;
	/* Maximum number of sockets before reallocation needed */
	int max;
} sock_arr;

int create_sock_arr(sock_arr *arr);
int add_new_client(sock_arr *arr, int fd);
int remove_client(sock_arr *arr, int idx);

int proc_msg(sock_arr *arr, int idx);

int proc_config(int fd);

int proc_asset_load(int fd);
int proc_asset_show(int fd);
int proc_asset_hide(int fd);
int proc_asset_tform(int fd);
int proc_asset_destroy(int fd);

int proc_action_load(int fd);
int proc_action_set_frame(int fd);
int proc_action_set_weight(int fd);
int proc_action_destroy(int fd);

int proc_viewpoint_tform(int fd);
int proc_viewpoint_set_fov(int fd);

#endif

