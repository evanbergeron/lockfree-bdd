---
layout: default
---


Evan Bergeron, Kevin Zheng

## Summary

We implemented a parallel binary decision diagram library, focusing on spatial locality and cache coherence. The key data structure is a collection of lockfree hash tables that doubles as a memory allocator, garbage collector, and directed graph.

## Background

Binary decision diagrams (BDDs) are directed graphs that represent boolean functions. Once constructed, these graphs provide constant time equivalence checking. Unfortunately, constructing these graphs can be costly.

The graphs are built incremently, usually taking a couple at a time and combining them. We focused on parallelizing this combination step.

This is tough for a number of reasons.

* *Low arithmetic intensity*: at each step, there's only a couple of pointer redirections.
* *Lack of spatial locality*: graph traversal can access a wide range of memory addresses and the structure of the input graphs is unpredictable.
* *Data dependency*: the result of a node is dependent on its children.

The low arithmetic intensity is somewhat inherent to the problem.

## Approach

In all implementations, we maintain a unique table: a key-value store that ensures no duplicate nodes are created.


### First approach - DFS

Our original serial implementation DFS'd on the graph using the if-then-else normal form operation as described in [2]. We parallelized this using Cilk+ in a fashion similar to [9, 10]. Pseudocode is presented below:

{% highlight c++ %}
// DFS approach
bdd combine(bdd f, bdd g, bdd h) {
  (fv, gv, hv) = left child;
  (fvn, gvn, hvn) = left child;
  result->left = spawn_task combine(fv, gv, hv);
  result->right = combine(fvn, gvn, hvn);
  sync;
  return result;
}
{% endhighlight %}

A constant size worker pool is maintained (in our case, using Cilk+). There is data parallelism in the left and right children of each node, so compute each subgraph in parallel.

In this implementation, a lossy memoization cache is shared between workers to avoid duplicate work. Additionally, the DAG and unique table are merged as described in [2] to reduce memory footprint.

This implementation provides a nice solution to the data dependency issue; simply compute from the leaves up. This makes certain canonicity checks very straightforward and yields elegent, readable code. Unfortunately, does very little to address the spatial locality issue.

## Improving Spatial Locality

And so begins the long journey to spatial locality.

### Open-Addressing

Our initial unique table used separate chaining with linked lists in each bucket. After decided that we wanted to focus on memory locality, we switched a linear-probing, open-addressing scheme.

In a parallel context, this hash table must be able to be read and written to concurrently. Our final implementation is heavily based on [11].

### Node Managers

Our original graph implementation used a simple recursive formulation:

{% highlight c++ %}
struct bdd_node {
  int varid;
  bdd_node *left;
  bdd_node *right;
};
{% endhighlight %}

with each of these nodes being malloc'd individually. To improve spatial locality, we implemented **node managers**, as described in [3, 4].

At init time, the node managers pre-allocate large blocks of memory. When the user requests a new node allocation, they must provide a variable id. Node with similar variable ids are grouped together.

The intent was to use the node managers in conjunction with a BFS-style tree traversal. The BFS operation is significantly trickier than DFS, involving two separate phases. At this point, we only had the first phase correctly implemented.

### A new struct definition

At each step of either a BFS or DFS style traversal, the expansion algorithm must request the variable ids of each child node. [3] presents a nontraditional struct definition that inlines the children variable ids to preserve locality of access. We did something similar, inlining packed pointers to the left and right children.

{% highlight c++ %}
struct bdd_ptr_packed {
  uint16_t varid;
  uint32_t idx;
} __attribute__((packed));

struct bdd {
  bdd_ptr_packed lo;    // 6 bytes
  bdd_ptr_packed hi;    // 6 bytes
  uint16_t varid;       // 2 bytes
  uint16_t refcount;    // 2 bytes
};
{% endhighlight %}

Only 16 bytes in total, matching [3]'s size. Conveniently, this lets us use a 128-bit compare_and_swap.

The packed pointers are used as unique identifiers into our unique table. Since we define our own memory allocation scheme, we can uniquely identify nodes by their variable id and internal position.

### BFS

This was hard. It took us several tries to correctly implement this, though it was our intent for a while. All of the memory locality design decisions made above were made with BFS in mind.

Recall that in the BDD graphs, the value of a node is dependent on its childrens' values. In a DFS context, we compute the value of the children prior to the parent, making reduction straightforward. In BFS, we need two phases: an expansion and a reduction phase.

Our final implementation uses the ite formulation of bfs, similar to [8]. Our understanding is that this incurs more memory accesses than the more modern (op, node, node) implementations as in [4, 5, 6]. We ultimately chose this approach because of its consistency with our dfs implementation, which allowed for a good deal of code reuse.

We maintain a mapping from bdd_node triples to a final bdd_result. We maintain a dynamically-resized list for each variable. The elements of the list are 4-tuples: 3 bdd_ptrs (a key) and 1 bdd node (a result).

Pseudocode is presented below.

{% highlight c++ %}
void bfs_expand(f, g, h) {
  make new bdd_node r;
  create_request(f, g, h, r);
  for each varid
    for each request of this varid
      expand lo and hi, add to appropriate queue
}
{% endhighlight %}

{% highlight c++ %}
void bfs_reduce() {
  for each varid, starting from the bottom
    for each request of this varid
      if either child is a forwarding node,
        grab the node they forward to
      if lo == hi, make yourself a forwarding node
        to one of them
      if your value is already in the unique table,
        forward to that
      otherwise, insert into the unique_table
}
{% endhighlight %}


## Final Implementation

Our implementation has two key data structures:

* **The unique table**: a collection of lockfree hash tables that double as a directed graphs and memory allocators

* **The memo table**: an additional auxiliary memoization table

### The Unique Table

Our unique table is an array of hash tables. We have a num_var parameter; for each variable id, there is a separate hash table. This helps ensure spatial locality.

The hash tables use open-addressing with linear probing. A compare_and_swap is used while probing across. These hash tables are filled with the following struct:

{% highlight c++ %}
struct bdd {
  bdd_ptr_packed lo;  // 6 bytes
  bdd_ptr_packed hi;  // 6 bytes
  uint16_t varid;     // 2 bytes
  uint16_t refcount;  // 2 bytes
}
{% endhighlight %}

This struct represents a node in our BDDs. The lo and hi pointers represent edges to other nodes. The varid identifies which boolean variable this node is associated with. The refcount aids in garbage collection.

Since the pointers to lo and hi lead to another location in this data structure, we've effectively embedded the directed graphs directly into the hash tables.

Note that the bdd_ptr_packed structs are inlined. This is done with the intent of reducing memory accesses. Our bdd_apply function needs to know the id of the two children nodes. These structs contain the varids, so we avoid an additional dereference of a value that's likely to be a cache miss.

These hash tables support essentially one operation: lookup_or_insert. This is a combined find and put, essentially. We hash the key and linearly probe across using compare and swap. If we find the key we're looking for, we atomically increment the refcount. If we find an empty space, we insert here.

## Results

This section is in progress. We finally have a working implementation and are currently benchmarking.

## Future Work

There a handful of optimizations on sequential BDDs that we didn't get the chance to implement. Complement edges and standard triples as described in [2] would be the first step.

The size of the resulting BDD can be highly dependent on the ordering of variables. For this reason, some BDD libraries provide heuristics for dynamic variable reordering.

Additionally, we'd be interested in implementing a hybrid DFS/BFS traversal in the spirit of [4, 5, 6].

## Lessons Learned

## References

[1] Randal E. Bryant. Graph-Based Algorithms for Boolean Function Manipulation.

[2] Karl S. Brace, Richard L. Rudell, Randal E. Bryant. Efficient Implementation of a BDD Package.

[3] Jagesh V. Sanghavi, Rajeev K. Ranjan, Robert K. Brayton, Alberto Sangiovanni-Vincentelli. High Performance BDD Package By Exploiting Memory Hierarchy.

[4] Bwolen Yang, Yirng-An Chen, Randal E. Bryant, David R. O'Hallaron. Space-and-Time-Efficient BDD Construction via Working Set Control.

[5] Bwolen Yang and David R. O'Hallaron. Parallel Breath-First BDD Construction.

[6] Yirng-An Chen, Bwolen Yang, and Randal E. Bryant. Breath-First with Depth-First BDD Construction: A Hybrid Approach.

[7] Tony Stornetta and Forrest Brewer. Implementation of an Efficient Parallel BDD Package.

[8] Parnav Ashar and Matthew Cheong. Efficient Breath-First Manipulation of Binary Decision Diagrams.

[9] Tom Van Dijk, Alfons Laarman, and Jaco van de Pol. Multi-Core BDD Operations for Symbolic Reachability.

[10] Yuxiong He. [Multicore-enabling a Binary Decision Diagram algorithm](https://software.intel.com/en-us/articles/multicore-enabling-a-binary-decision-diagram-algorithm).

[11] Jesh Preshing. [The World's Simplest Lockfree Hash Table](http://preshing.com/20130605/the-worlds-simplest-lock-free-hash-table/)

* [Checkpoint](http://evan.sh/lockfree-bdd/checkpoint.html)
* [Proposal](http://evan.sh/lockfree-bdd/proposal.html)

Equal work was performed by both project members.

<a href="https://github.com/evanbergeron/lockfree-bdd" class="github-corner"><svg width="80" height="80" viewBox="0 0 250 250" style="fill:#151513; color:#fff; position: absolute; top: 0; border: 0; right: 0;"><path d="M0,0 L115,115 L130,115 L142,142 L250,250 L250,0 Z"></path><path d="M128.3,109.0 C113.8,99.7 119.0,89.6 119.0,89.6 C122.0,82.7 120.5,78.6 120.5,78.6 C119.2,72.0 123.4,76.3 123.4,76.3 C127.3,80.9 125.5,87.3 125.5,87.3 C122.9,97.6 130.6,101.9 134.4,103.2" fill="currentColor" style="transform-origin: 130px 106px;" class="octo-arm"></path><path d="M115.0,115.0 C114.9,115.1 118.7,116.5 119.8,115.4 L133.7,101.6 C136.9,99.2 139.9,98.4 142.2,98.6 C133.8,88.0 127.5,74.4 143.8,58.0 C148.5,53.4 154.0,51.2 159.7,51.0 C160.3,49.4 163.2,43.6 171.4,40.1 C171.4,40.1 176.1,42.5 178.8,56.2 C183.1,58.6 187.2,61.8 190.9,65.4 C194.5,69.0 197.7,73.2 200.1,77.6 C213.8,80.2 216.3,84.9 216.3,84.9 C212.7,93.1 206.9,96.0 205.4,96.6 C205.1,102.4 203.0,107.8 198.3,112.5 C181.9,128.9 168.3,122.5 157.7,114.1 C157.9,116.9 156.7,120.9 152.7,124.9 L141.0,136.5 C139.8,137.7 141.6,141.9 141.8,141.8 Z" fill="currentColor" class="octo-body"></path></svg></a><style>.github-corner:hover .octo-arm{animation:octocat-wave 560ms ease-in-out}@keyframes octocat-wave{0%,100%{transform:rotate(0)}20%,60%{transform:rotate(-25deg)}40%,80%{transform:rotate(10deg)}}@media (max-width:500px){.github-corner:hover .octo-arm{animation:none}.github-corner .octo-arm{animation:octocat-wave 560ms ease-in-out}}</style>
