---
layout: default
---

This is Evan Bergeron and Kevin Zheng's 15-418 final project site.

## Summary

We are going to produce two binary decision diagram library implementations, one with fine-grained locking, and one lockfree. Everything will be written in C++.

## Background

Binary decision diagrams are data structures used to represent boolean functions. We'll give some background on boolean functions and then discuss how to represent them computationally.

### The Math

Recall that a boolean function is a function of the form

$$f : \textbf{2}^k \rightarrow \textbf{2}$$

where $\textbf{2} = \\{ 0, 1 \\}$. These can be represented as rooted binary trees. Take some ordering on the input variables. Set $x_0$ to be the root of the tree. Then the left child of the root corresponds with the assignment $x_0 = 0$ and the right child means $x_0 = 1$. Continue this inductively down to the leaves.

The leaves then represent a complete assignment of the input variables. Call this assignment $\textbf{v}$. Set each leaf to be $f(\textbf{v})$.

Some of the subtrees of this complete binary tree may be identical (isomorphic). Starting at the root and moving inductively downward, join subtrees that are isomorphic. If both subtrees of a node are isomorphic, remove this node (in a sense, this variables does not matter).

The result of this process is called a *reduced ordered binary decision diagram*, or ROBDD. When people say BDD, they usually mean ROBDD.

ROBDDs have a number of useful properties, not the least of which is *canonicity*. Which is to say, given a boolean function $f$ and an ordering on the variables $\leq$, there exists a unique ROBDD representing $f$ with the $\leq$ ordering.

Critically, this means that boolean function equivalence can be determined by structural equivalence.

### The Data Structure

How are BDDs often implemented? Naturally, there are number of ways. Building an ordered, non-reduced BDD is easy. Supporting the merging operations quickly is the main task. Specifically, there are two main tasks

* Merge isomorphic subgraphs
* Eliminate nodes whose children are ismorphic.

Typically, when building a ROBDD, we reduce as we go, rather than build the exponentially large data structure and then reduce. A hash table supports these operations well.

A typical implementation will maintain a "unique-table" - prior to adding a node $f = (g, v, h)$, we check if the node we're adding is already in the table. If so, proceed, otherwise add. This table can then be built recursively from the leaves up.

In general, of course, the BDDs are exponential in size, even if reduced. This provides an avenue for parallelism. Building the unique table in parallel may provide a good deal of speedup.

For this reason, building a performant parallel hash table is in order. This is the central challenge of the project.

## The Challenge

We intend to build a performant, parallel hash table as the backbone of this project. We intend to offer a handful of solutions and compare performance across implementations.

The main goal is a separate-chained lock-free hash table. Evidence suggests that hash tables are a data structure that benefit greatly from non-blocking implementations. We'll also investigate fine-grained locking with separate-chaining, along with various probing strategies.

Further, paralleling BDD construction seems nontrivial, so even once we finish implementing a lockfree hash table, there will be cleverness required in its usage.

## Resources
We intend on starting from scratch, though with some papers and code as reference. In particular, we expect the Harris paper "A Pragmatic Implementation of Non-Blocking Linked Lists" to be invaluable, as it has been for other students in the past. There are also a number of open-source BDD libraries out there, which will serve as reference points for sequential implementations.

Almost all development will be done locally, though we may look into using different machines for varying memory consistency models.

Further, we may look into performance testing on the Xeon Phis.

## Goals and Deliverables

### Plan to Achieve
* Fine-grained locking implementations of linked list, separately-chained hash table, and BDD library
* Lock-free implementations of linked list, separately-chained hash table, and BDD library
* Benchmarks investigating performance across both implementations
* A handful of canonical BDD examples, specifically nqueens

Our understanding of the project expectations is that successful completion of the above will constitute an A on the project. Further areas of exploration include:

### Hope to Achieve
* Increasingly clever hash tables, perhaps using binary trees instead of linked lists in each bucket.
* Investigate correctness on other memory consistency models.
* Investigate porting the library to other languages, particular python or OCaml.
* Investigate alternative hash table implementations and various hash table optimizations
* Investigate running on the Xeon Phi

We haven't yet decided if we'd like to use Michael Sullivan's RMC project yet. This is one of the main tasks to be decided in the coming week. It's unclear whether this would make the project easier or harder.

## Platform Choice
We'll be using C++, as we intend to build a C++ BDD library. Our target audience is a CPU C++ software programmer, so this is a natural platform choice.

## Schedule
We have 5 weeks for the project. Broadly speaking, we have 6 main tasks. Fine-grained and lock-free versions of a linked list, a hash table, and the full BDD library.

Each list contains the list of tasks we expect to complete by the end of each week.

### Week 1
* Linked list with fine-grained locks
* Hash table with fine-grained locks
* Nontrivial progress on BDD library implementation (perhaps initially using builtin data structures)
* Determine if we'll use the RMC project
* Have thorough understanding of BDD implementation

### Week 2
* Complete BDD library with fine-grained locks
* Substantial progress on lock-free linked list implementation

### Week 3
* Substantial progress on lock-free hash table
* Complete lock-free linked list implementation

### Week 4
* Complete lock-free hash table
* Complete lock-free BDD library implementation

### Week 5
* Example code written
* Performance comparison
* Writeup complete

<a href="https://github.com/evanbergeron/lockfree-bdd" class="github-corner"><svg width="80" height="80" viewBox="0 0 250 250" style="fill:#151513; color:#fff; position: absolute; top: 0; border: 0; right: 0;"><path d="M0,0 L115,115 L130,115 L142,142 L250,250 L250,0 Z"></path><path d="M128.3,109.0 C113.8,99.7 119.0,89.6 119.0,89.6 C122.0,82.7 120.5,78.6 120.5,78.6 C119.2,72.0 123.4,76.3 123.4,76.3 C127.3,80.9 125.5,87.3 125.5,87.3 C122.9,97.6 130.6,101.9 134.4,103.2" fill="currentColor" style="transform-origin: 130px 106px;" class="octo-arm"></path><path d="M115.0,115.0 C114.9,115.1 118.7,116.5 119.8,115.4 L133.7,101.6 C136.9,99.2 139.9,98.4 142.2,98.6 C133.8,88.0 127.5,74.4 143.8,58.0 C148.5,53.4 154.0,51.2 159.7,51.0 C160.3,49.4 163.2,43.6 171.4,40.1 C171.4,40.1 176.1,42.5 178.8,56.2 C183.1,58.6 187.2,61.8 190.9,65.4 C194.5,69.0 197.7,73.2 200.1,77.6 C213.8,80.2 216.3,84.9 216.3,84.9 C212.7,93.1 206.9,96.0 205.4,96.6 C205.1,102.4 203.0,107.8 198.3,112.5 C181.9,128.9 168.3,122.5 157.7,114.1 C157.9,116.9 156.7,120.9 152.7,124.9 L141.0,136.5 C139.8,137.7 141.6,141.9 141.8,141.8 Z" fill="currentColor" class="octo-body"></path></svg></a><style>.github-corner:hover .octo-arm{animation:octocat-wave 560ms ease-in-out}@keyframes octocat-wave{0%,100%{transform:rotate(0)}20%,60%{transform:rotate(-25deg)}40%,80%{transform:rotate(10deg)}}@media (max-width:500px){.github-corner:hover .octo-arm{animation:none}.github-corner .octo-arm{animation:octocat-wave 560ms ease-in-out}}</style>
