# Make Friends Analysis

## Data
Fill out the remain parts of the following tables.
|            | CPU EPS |
|------------|---------|
| Instructor | 5671.61 |
| Student    | 499.71  |

|        | Scaling |
|--------|---------|
| I-to-S |11.34980289|

Scale the instructor numbers with the I-to-S scaling.
| File           | I People | I Con   | I Add Con | I Time (ns)    | S Con   | S Add Con | S Time (ns) |
|----------------|----------|---------|-----------|----------------|---------|-----------|-------------|
| friends1.csv   | 10       | 19      | 0         | 770,368        | 19      | 0         | 41,202      |
| friendssm.csv  | 10       | 12      | 1         | 459,667        | 12      | 1         | 18,000      |
| friendsmed.csv | 1,000    | 3,927   | 4         | 117,562,677    | 3,926   | 4         | 2,454,599   |
| friendslg.csv  | 100,000  | 416,102 | 29        | 10,381,538,433 | 416,102 | 29        | 436,730,361 |
Instructor times are scaled by 5671.61 / 499.71. The provided program reports 3,926 connections for friendsmed.csv, while the instructor table lists 3,927.

## Analysis


### What seems to be the growth rate for your Friend Groups structure in terms of number of people?
The measured growth is somewhat faster than linear. From the medium
to the large input, the number of people increases by 100 times,
while the measured runtime increases by about 178 times.
The number of connections also increases, so runtime depends on
both people and connections, not just people.

With n people and m connections, the expected time is
O((n + m) log n), assuming average constant-time hash-table
operations and bounded name lengths. Union by size keeps the
union-find trees at logarithmic height. This implementation does
not use path compression.



### Does it appear to grow faster or slower than expected?
The results are consistent with the expected near-linear to
O(n log n) growth when the number of connections is proportional
to the number of people. The available measurements are not
enough to establish a precise empirical growth rate.

### If your implementation is deviating, why do you believe your implementation is deviating from what might be expected?
Larger inputs require more allocations, hash-table expansions,
string hashing, and parent traversals. Cache behavior and
background system activity can also affect runtime.
Each reported time is from a single run, and small inputs are
especially sensitive to timing variation.

### What do you believe is the dominant structure/algorithm in your implementation?
Processing the input friendships dominates the work. Each
friendship requires name lookups in the hash map and a union
operation in the union-find structure.

The map uses separate chaining and doubles its bucket count
when needed. The union-find structure uses union by size.

To generate the proposed friendships, the program scans all
people, selects one representative per group, and connects
those representatives in a chain. For k groups, this produces
exactly k - 1 new friendships, and each selected person appears
at most twice. Space usage is O(n), excluding the input data
stored by the provided driver.
