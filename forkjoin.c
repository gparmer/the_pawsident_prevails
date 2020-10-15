#include <pthread.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include <forkjoin.h>

void
panic(char *errstr)
{
	printf("%s\n", errstr);
	exit(EXIT_FAILURE);
}

struct fork_metadata {
	pthread_t id;
	fork_fn_t fn;
	void     *context;
	size_t    data_sz, offset;
};

struct fork_metadata *forks;
int nforks;

void *
fork_out_fn(void *_data)
{
	struct fork_metadata *d = _data;
	size_t i;

	for (i = 0; i < d->data_sz; i++) {
		d->fn(d->context, d->offset + i);
	}

	return NULL;
}

void
forkjoin(int nfork, fork_fn_t fn, void *context, size_t data_sz)
{
	int i;
	size_t sz = (data_sz / nfork);
	struct fork_metadata *forks;
	;
	forks = malloc(nfork * sizeof(struct fork_metadata));
	assert(forks);

	/* Fork out computations for each set of iterations */
	for (i = 0; i < nfork; i++) {
		/* last iteration? allocate the rest! */
		if (i == nfork - 1) sz = data_sz;

		forks[i] = (struct fork_metadata) {
			.fn      = fn,
			.context = context,
			.data_sz = sz,
			.offset  = i * sz
		};
		/* We handed away sz items */
		data_sz -= sz;

		if (pthread_create(&forks[i].id, NULL, fork_out_fn, &forks[i])) {
			panic("Pthread create failure");
		}
	}

	/* Join them back in, then resume execution. */
	for (i = 0; i < nfork; i++) {
		if (pthread_join(forks[i].id, NULL)) {
			panic("Pthread join failure");
		}
	}
}
