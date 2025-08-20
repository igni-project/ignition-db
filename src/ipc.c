#include "ipc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int sockarr_create(struct sockarr *arr)
{
	arr->fds = malloc(sizeof(int));
	arr->count = 0;
	arr->limit = 1;

	return 0;
}

int sockarr_append(struct sockarr *arr, int fd)
{
	/* The reallocated address is audited before use */
	int *new_addr;

	if (arr->count >= arr->limit)
	{
		arr->limit *= 2;
		new_addr = realloc(arr->fds, arr->limit);

		if (!new_addr)
		{
			perror("sockarr_append() failed: could not reallocate memory");
			return -1;
		}

		arr->fds = new_addr;
	}
	
	arr->fds[arr->count] = fd;

	++arr->count;

	return 0;
}

int sockarr_remove(struct sockarr *arr, size_t idx)
{
	/* The reallocated address is audited before use */
	int *new_addr;

	--arr->count;

	memcpy(
		&arr->fds[idx],
		&arr->fds[idx + 1],
		(arr->count - idx) * sizeof(int)
	);

	if (arr->count < arr->limit / 2)
	{
		arr->limit /= 2;
		new_addr = realloc(arr->fds, arr->limit);

		if (!new_addr)
		{
			perror("remove_client() failed: could not allocate memory");
			return -1;
		}

		arr->fds = new_addr;
	}

	return 0;
}

void sockarr_destroy(struct sockarr arr)
{
	free(arr.fds);
}

