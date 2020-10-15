#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include <forkjoin.h>
#include <mapreduce.h>

void *
dumb_prime(void *_n){
	long n = (long)_n;
	long i;
	long *output = malloc(sizeof(long));

	assert(output);
	*output = 1;
	for(i = 2; i < n; i++){
		if(n % i == 0){
			/* not a prime! */
			*output = 0;
			break;
		}
	}

	return output;
}

/********** forkjoin implementation **********/

struct forkjoin_context {
	void **input;
	void **output;
};

void
prime_fork(void *context, size_t offset)
{
	struct forkjoin_context *ctxt = context;

	ctxt->output[offset] = dumb_prime(ctxt->input[offset]);
}

/* Just kick off the forkjoin task! */
void
use_forkjoin(int parallelism, void *input, int input_sz)
{
	struct forkjoin_context context;
	int i;
	long is_prime;
	int nprimes = 0;

	context = (struct forkjoin_context) {
		.input = input,
		.output = malloc(input_sz * sizeof(void *))
	};

	assert(context.output);
	memset(context.output, 0, input_sz * sizeof(void *));

	forkjoin(parallelism, prime_fork, &context, input_sz);

	/* This is not in parallel!!! */
	for (i = 0 ; i < input_sz; i++) {
		is_prime = *(long *)context.output[i];
		if (is_prime) nprimes++;
		free(context.output[i]);
	}

	printf("Forkjoin parallelism %d: %d out of %d are primes!\n", parallelism, nprimes, input_sz);
}

/********** mapreduce implementation **********/

void *
prime_map(void *context, void *value)
{
	(void)context;

	return dumb_prime(value);
}

struct prime_reduce_agg {
	long primes, total;
};

struct prime_reduce_agg *
new_prime_agg(long total, long primes)
{
	struct prime_reduce_agg *agg;

	agg = malloc(sizeof(struct prime_reduce_agg));
	assert(agg);
	*agg = (struct prime_reduce_agg) {
			.total  = total,
			.primes = primes
	};

	return agg;
}

void *
prime_reduce_init(void *context)
{
	(void)context;

	return new_prime_agg(0, 0);
}

void
prime_reduce(void *context, void *aggregate, void *newdata)
{
	struct prime_reduce_agg *agg = aggregate;
	long is_prime = *(long *)newdata;
	(void)context;

	assert(agg);
	*agg = (struct prime_reduce_agg) {
		.total  = agg->total  + 1,
		.primes = agg->primes + is_prime
	};
}

void
prime_combine(void *context, void *agg1, void *agg2)
{
	struct prime_reduce_agg *result = agg1;
	struct prime_reduce_agg *other  = agg2;
	(void)context;

	assert(agg1 && agg2);
	*result = (struct prime_reduce_agg) {
		.total  = result->total  + other->total,
		.primes = result->primes + other->primes,
	};
}

/* Just kick off the mapreduce task! */
void
use_mapreduce(int parallelism, void *input, int input_sz)
{
	struct prime_reduce_agg *ret;

	ret = mapreduce(parallelism, (void **)input, input_sz, NULL,
			prime_map, prime_reduce_init, prime_reduce, prime_combine);
	printf("Mapreduce parallelism %d: %ld out of %ld are primes!\n", parallelism, ret->primes, ret->total);
	free(ret);
}

/********** main implementation **********/

void
usage(char *bin)
{
	printf("Usage: %s <# search up to> <parallelism> <fj|mr>\n", bin);
	exit(EXIT_FAILURE);
}

int
main(int argc, char *argv[])
{
	long *input;
	int i, input_sz, parallelism;
	int fj = 0, mr = 0;
	(void)argc; (void)argv;

	/* Just parse the arguments to the program */
	if (argc != 4) usage(argv[0]);
	input_sz = atoi(argv[1]);
	parallelism = atoi(argv[2]);
	if (input_sz <= 0) usage(argv[0]);
	if (parallelism <= 0) usage(argv[0]);
	if (!strncmp(argv[3], "mr", 2)) mr = 1;
	if (!strncmp(argv[3], "fj", 2)) fj = 1;

	/* lets make the input! */
	input = malloc(sizeof(long) * input_sz);
	assert(input);
	for (i = 0; i < input_sz; i++) {
		input[i] = i;
	}

	if (mr) use_mapreduce(parallelism, input, input_sz);
	if (fj) use_forkjoin(parallelism, input, input_sz);

	printf("🐶 endorsed this message\n");
	free(input);

	return 0;
}
