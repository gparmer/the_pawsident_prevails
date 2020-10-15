#ifndef MAPREDUCE_H
#define MAPREDUCE_H

/***
 * Map-reduce!
 *
 *
 */

/**
 * Convert one of the values into the output type. This is a simple
 * map on the data. The context is passed in as the context to
 * mapreduce.
 */
typedef void *(*map_fn_t)(void *context, void *value);
/**
 * Your own function to combine results. You either combine the
 * `newdata` into the `aggregate`, or if `newdata == NULL &&
 * aggregate2 != NULL`, you combine the aggregates together. The
 * context is passed in as the context to mapreduce.
 */
typedef void (*reduce_fn_t)(void *context, void *aggregate, void *newdata);
typedef void (*combine_fn_t)(void *context, void *aggto, void *aggfrom);
typedef void *(*reduce_init_fn_t)(void *context);

void *mapreduce(int parallelism, void *input[], size_t data_sz, void *context, map_fn_t map, reduce_init_fn_t reduce_init, reduce_fn_t reduce, combine_fn_t combine);

#endif	/*  MAPREDUCE_H */
