#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <forkjoin.h>
#include <mapreduce.h>

struct mapreduce_ctxt {
	map_fn_t    map;
	reduce_fn_t reduce;
	combine_fn_t combine;
	reduce_init_fn_t rinit;
	size_t      data_sz;
	size_t      reduce_fanout;
	void       *ctxt;
	void       **input;
	void       **output;
	void       **reduce_output;
};

/* Just convert between the function types of mapreduce and forkjoin */
static void
fj2map_type_conversion_fn(void *context, size_t offset)
{
	struct mapreduce_ctxt *mr = context;

	mr->output[offset] = mr->map(mr->ctxt, mr->input[offset]);
}

static void
fj2reduce_type_conversion_fn(void *context, size_t offset)
{
	struct mapreduce_ctxt *mr = context;
	size_t output_off = (mr->data_sz / mr->reduce_fanout) * offset;
	void *agg;
	size_t i;

	agg = mr->rinit(context);

	for (i = 0; i < mr->data_sz / mr->reduce_fanout; i++) {
		mr->reduce(mr->ctxt, agg, mr->output[output_off + i]);
		free(mr->output[output_off + i]);
	}

	mr->reduce_output[offset] = agg;
}

void *
mapreduce(int parallelism, void *input[], size_t data_sz, void *context,
	  map_fn_t map, reduce_init_fn_t reduceinit, reduce_fn_t reduce, combine_fn_t combine)
{
	void **output  = malloc(data_sz * sizeof(void *));
	void **reducin = malloc(parallelism * sizeof(void *));
	struct mapreduce_ctxt ctxt;
	void *result = NULL;
	int i;

	assert(parallelism > 0);
	assert(reducin && output);
	ctxt = (struct mapreduce_ctxt) {
		.map     = map,
		.reduce  = reduce,
		.rinit   = reduceinit,
		.combine = combine,
		.input   = input,
		.output  = output,
		.data_sz = data_sz,
		.ctxt    = context,
		.reduce_output = reducin,
		.reduce_fanout = parallelism
	};

	memset(output, 0, sizeof(void *) * data_sz);
	memset(reducin, 0, sizeof(void *) * parallelism);

	/* parallel computation to transform each data item */
	if (map) forkjoin(parallelism, fj2map_type_conversion_fn, &ctxt, data_sz);
	else memcpy(output, input, data_sz * (sizeof(void *)));

	if (!reduce) {
		free(reducin);
		return output;
	}
	assert(reduceinit && combine);

	/* Aggregate from the data input values */
	forkjoin(parallelism, fj2reduce_type_conversion_fn, &ctxt, parallelism);

	/* combine the aggregates! */
	result = reducin[0];
	for (i = 1; i < parallelism; i++) {
		assert(reducin[i]);
		combine(context, result, reducin[i]);
		free(reducin[i]);
	}
	free(output);
	free(reducin);

	return result;
}
