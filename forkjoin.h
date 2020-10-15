#ifndef FORKJOIN_H
#define FORKJOIN_H

/***
 * So-called "fork-join" parallelism mimics the structure of a for
 * loop, but where each iteration can be done in parallel! It cannot
 * *generally* be replaced for any "for" loop, but for loops where
 * each iteration is *independent* from the others can use this. The
 * "fork" here is NOT the UNIX fork. Instead it is closer, by analogy,
 * to a "fork in the road".
 *
 * This is a common parallelism technique used in OpenMP, Cilk, Intel
 * TCB, and Apple GCD. There are also java frameworks for doing the
 * same.
 */

/* The function the user defines for per-iteration computation */
typedef void (*fork_fn_t)(void *context, size_t offset);
void forkjoin(int nfork, fork_fn_t fn, void *context, size_t data_sz);

#endif	/* FORKJOIN_H */
