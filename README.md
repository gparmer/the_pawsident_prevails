# The Great Pawsident

In the chaos of the lock-free revolution, and the return to control under the dictator of mutually exclusive ordering, and, finally, the aristocracy of fine-grained and read-write-based bureaucracy, the citizens have grown discontent.
The shared memory of society is powerful, but we require some means for providing citizens an *easier* way to interact with it!
The final nail in the coffin was the [specter](https://en.wikipedia.org/wiki/Spectre_(security_vulnerability)) of deadlock.
Normal citizens can no longer manage and rely on the deep bureaucracy of the government.

There is hope!

The aristocracy of fine-grained and read-write locks are ceding power to a democratically elected representative!
Among those candidates are the [exotic](https://en.wikipedia.org/wiki/Joe_Exotic) -- peddling rare animal and tiger zoos that have free publicity from Netflix, and professional fake tanners that distinguish themselves with an orange glow and a dislike of those that are not orange, and most importantly, a most pawticularly adorable candidate.

![Penny for pawsident!](penny_for_pawsident.jpg)

## Penny's Pawtform

Penny's running on a non-traditional platform of *hiding the complexity of locks* from the citizen application!
Her key initiatives:

1. Pets-care for all.
1. Shed the red tape of mutual exclusion and fine-grained locking.
1. forkjoin parallelism for freedom
1. mapreduce processing toward a better society

*Forkjoin* parallelism enables *data parallel* access to data.
Instead of a normal fork loop in which each iteration executes sequentially, what if we could execute each iteration in a separate thread, on a separate core!?
It is called "forkjoin" as when we hit the forkjoin in the code, we "fork out parallel computation", and when the "loop" is done, we "join together".
Note that read `fork`s are *not* used here, and the word "fork" means something close to a "fork in the road".

**Question 1:**
Will this work for any program, and any fork loop?
For which will it work?

*Mapreduce* is a technique common in *distributed systems* whereby we define a set of functions, most notably *map* and *reduce*.

**Question 2:**
Have you heard of these functions from other languages?
What do they often do?

Mapreduce will call the map function on each item in the input, thus generating a set of output items.
Then we will combine all of these to get a final results, a process called "reducing them".

**Question 3:**
What types of problems could be computed using these primitives?

Read through `main.c` and answer the following:

**Question 4:**
What is more complex about the forkjoin parallelism versus mapreduce?

**Question 5:**
What is more complex about the mapreduce parallelism versus forkjoin?

**Question 6:**
Run `sh test.sh`.
Anything surprising or unexpected?

**Question 7:**
Check out the implementation of `forkjoin` in `forkjoin.c`.
Can you think of any way this could be optimized around the thread operations?

## Penny's prerogative

Penny promises bringing parallel programming to all!
Unfortunately, she doesn't provide abstractions that everyone can use for *every problem*.
But she does bring abstractions that everyone can understand, and can use without the pains of history -- locks, deadlock, and lock-free programming.

Higher-level primitives for parallel programming exist, but they are all limited in their programming model in some way.
