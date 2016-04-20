---
layout: default
---

## Checkpoint

We're on schedule, though we have pivoted on our intended implementation rather dramatically. After further review of the literature, we've concluded that separate-chained hash tables aren't the most effiecent or practical hash table implementation for this use case. We've instead switched to open-addressing. As we previously intended for a lock-free linked list to be the backbone of our implementation, this is a significant change in direction.

It took a surprising amount of time to get up to speed on how to implement BDDs. We've read around 4-5 papers, and our currently implementation is a hybrid of two papers in particular: "Multi-Core BDD Operations for Symbolic Reachability", and "Efficient Implementation of a BDD Package."

We currently have a complete sequential implementation. The core operation is the ite function as described in "Efficient Implementation...". Our unique table uses the C++ built-in library, and our lossy memoization table works as described in "Multi-Core...".

We've implemented a number of small benchmarks, as well as nqueens.

We're part-way through working on parallelizing our memo table and unique table. We expect to have a complete parallel implementation within the next week. From there, we're planning on investigating cache performance.

Our currently implementation uses structs as bdd nodes. Every time a new node is created, we have a memory allocation. We expect this will be our performance issue to address. The BuDDy library represents BDD nodes as integers, indexing into an array - we expect to do something similar.

We also have a near-complete fine-grained-locking linked list implementation. Most of the work on this was done prior to further investigation of the literature. We don't expect to implement a lock-free linked list at this point, as we expect cache coherence to be a large performance issue for the BDD library. That being said, we can still finish the linked list and compare its performance to our open-addressing hash table.

Following is an adjusted schedule for the remainder of the project duration.

## Goals and Deliverables

### Plan to Achieve
* Fine-grained locking implementations of linked list, separately-chained hash table, and BDD library
* Lock-free implementations of open-addressing hash table, and BDD library
* Benchmarks investigating performance across both implementations
* A handful of canonical BDD examples, specifically nqueens

### Hope to Achieve
* Cache-friendly implementations of the memo table and unique table.
* Investigate wrapping the library as a python extension
* Investigate running on the Xeon Phi

## Schedule

### Week 3 (First Half)
* Finish reading up on cache-friendly implementations (Both Kevin and Evan)

### Week 3 (Second Half)
* Parallel unique table (Evan)
* Parallel memo table (Kevin)

### Week 4 (First half)
* Complete fine-grained locking linked list implementation (Evan)
* BFS version of ite (Evan)
* Track cache performance and make unique table more cache friendly (Kevin)

### Week 4 (Second half)

### Week 5 (First half)
* Demo finalized (both)
* Performance comparison between various implementations (both)

### Week 5 (Second half)
* Finish writeup (both)

## Planned Demo

We expect to provide graphs regarding speedup on an increasing number of cores. We currently have nice visualization tools for the BDDs we construct, so we can actually give a visualization of the data structure in memory. We can also run our nqueens implementation and show all the different configurations that work.

<a href="https://github.com/evanbergeron/lockfree-bdd" class="github-corner"><svg width="80" height="80" viewBox="0 0 250 250" style="fill:#151513; color:#fff; position: absolute; top: 0; border: 0; right: 0;"><path d="M0,0 L115,115 L130,115 L142,142 L250,250 L250,0 Z"></path><path d="M128.3,109.0 C113.8,99.7 119.0,89.6 119.0,89.6 C122.0,82.7 120.5,78.6 120.5,78.6 C119.2,72.0 123.4,76.3 123.4,76.3 C127.3,80.9 125.5,87.3 125.5,87.3 C122.9,97.6 130.6,101.9 134.4,103.2" fill="currentColor" style="transform-origin: 130px 106px;" class="octo-arm"></path><path d="M115.0,115.0 C114.9,115.1 118.7,116.5 119.8,115.4 L133.7,101.6 C136.9,99.2 139.9,98.4 142.2,98.6 C133.8,88.0 127.5,74.4 143.8,58.0 C148.5,53.4 154.0,51.2 159.7,51.0 C160.3,49.4 163.2,43.6 171.4,40.1 C171.4,40.1 176.1,42.5 178.8,56.2 C183.1,58.6 187.2,61.8 190.9,65.4 C194.5,69.0 197.7,73.2 200.1,77.6 C213.8,80.2 216.3,84.9 216.3,84.9 C212.7,93.1 206.9,96.0 205.4,96.6 C205.1,102.4 203.0,107.8 198.3,112.5 C181.9,128.9 168.3,122.5 157.7,114.1 C157.9,116.9 156.7,120.9 152.7,124.9 L141.0,136.5 C139.8,137.7 141.6,141.9 141.8,141.8 Z" fill="currentColor" class="octo-body"></path></svg></a><style>.github-corner:hover .octo-arm{animation:octocat-wave 560ms ease-in-out}@keyframes octocat-wave{0%,100%{transform:rotate(0)}20%,60%{transform:rotate(-25deg)}40%,80%{transform:rotate(10deg)}}@media (max-width:500px){.github-corner:hover .octo-arm{animation:none}.github-corner .octo-arm{animation:octocat-wave 560ms ease-in-out}}</style>
