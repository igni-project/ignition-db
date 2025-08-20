#include "main.h"
#include "debug/print.h"
#include "cmd/cmdline.h"
#include "render/gfxenv.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/time.h>
#include <sys/select.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>

#define FPS_CAP 60

int main(int argc, char *argv[], char *envp[])
{
	/* Server socket */
	struct sockaddr_un sv_addr;
	int srv_fd;

	/* Variables for accept() */
	struct sockaddr_un tmaddr = {0};
	socklen_t tmaddr_sz = 0;
	int new_fd;

	/* Set of all sockets */
	fd_set read_fds;
	int max_fd;

	/* keep main loop going */
	char running = 1;

	/* Socket activity */
	int activity;

	/* The 3d world */
	struct gfxenv gfxenv;

	/* Counter */
	int i;

	/* Command-line arguments */
	struct gengetopt_args_info args_info = {};

	/* Parse command line arguments */
	if (cmdline_parser(argc, argv, &args_info))
	{
		printf("Failed to parse command line arguments\n");
	}

	srv_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (srv_fd == -1)
	{
		perror("Failed to create server socket");
		return -1;
	}

	sv_addr.sun_family = AF_UNIX;
	strncpy(sv_addr.sun_path, args_info.domain_arg, sizeof(sv_addr.sun_path));
	if (bind(srv_fd, (struct sockaddr*)&sv_addr, sizeof(sv_addr)) == -1)
	{
		perror("Failed to bind server socket");
		return -1;
	}

	if (listen(srv_fd, 10) == -1)
	{
		perror("Failed to open server socket for listening");
		return -1;
	}

	/* Ignition may execute child processes on request. */
	i = 0;
	while (i < args_info.inputs_num)
	{
		if (args_info.verbose_flag)
		{
			printf("Executing %s\n", args_info.inputs[i]);
		}

		if (!fork()) break;

		if (execve(args_info.inputs[i], 0, envp) == -1) {
			printf("Failed to execute ");
			perror(args_info.inputs[i]);
			exit(EXIT_FAILURE);
		}

		++i;
	}

	/* Configure debug messages */
	if (args_info.verbose_flag)
	{
		db_verbose_enable();
	}

	if (create_gfxenv(&gfxenv) == -1)
	{
		return -1;
	}

	/* The file descriptor of the server socket is higher than all zero of the
	 * other active sockets. */
	max_fd = srv_fd;

	/* MAIN LOOP */
	while (running)
	{
		/* 	Set up socket file descriptor set and max descriptor.
			This code must occur every time a message is to be recieved. */
		FD_ZERO(&read_fds);
		FD_SET(srv_fd, &read_fds);
		i = 0;
		while (i < gfxenv.scene_count)
		{
			FD_SET(gfxenv.scenes[i].fd, &read_fds);

			if (gfxenv.scenes[i].fd > max_fd)
			{
				max_fd = gfxenv.scenes[i].fd;
			}

			++i;
		}

		/* Wait until a socket is active */
		activity = select(max_fd + 1, &read_fds, 0, 0, 0);

		if (activity == -1 && errno != EINTR)
		{
			perror("Socket select failed");
		}

		/* Activity on the server socket means a new connection */
		if (FD_ISSET(srv_fd, &read_fds))
		{
			new_fd = accept(srv_fd, (struct sockaddr*)&tmaddr, &tmaddr_sz);
			if (new_fd == -1)
			{
				perror("Failed to accept client");
			}
			else
			{
				gfxenv_new_client(&gfxenv, new_fd);
			}
		}

		/* Execute incoming SUP commands */
        i = gfxenv.scene_count - 1;
        while (i != -1)
        {
            if (FD_ISSET(gfxenv.scenes[i].fd, &read_fds))
            {
				gfxenv_exec(&gfxenv, i);
            }

            --i;
        }
	}

	/* Clean up before exit */
	destroy_gfxenv(gfxenv);

	return 0;
}

