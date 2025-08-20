#ifndef IPC_H
#define IPC_H 1

#include <stddef.h>

struct sockarr
{
	int *fds;
	size_t count;
	size_t limit;
};

int sockarr_create(struct sockarr *arr);
int sockarr_append(struct sockarr *arr, int fd);
int sockarr_remove(struct sockarr *arr, size_t idx);
void sockarr_destroy(struct sockarr arr);

#endif

