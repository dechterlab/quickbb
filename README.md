# QuickBB
This repository mirrors [software page from Professor Vibhav Gogate at UT Dallas](http://www.hlt.utdallas.edu/~vgogate/quickbb.html).

### Papers
* Vibhav Gogate and Rina Dechter, [“A complete Anytime Algorithm for Treewidth,”](http://www.hlt.utdallas.edu/~vgogate/papers/uai04.pdf) In 20th Conference on Uncertainty in Artificial Intelligence (UAI), 2004.

### Usage
```
$ quickbb [option1] [option2] –cnffile CNFFILE
```
A description of the (input) CNF format is given [here](http://www.cs.ubc.ca/~hoos/SATLIB/Benchmarks/SAT/satformat.ps). 
Any graph can be written in this format. For example, a chain 1-2-3 can be specified as:
```
p cnf 3 2
1 2 0
2 3 0
```
The first line says that we are specifying the graph using 3 variables and 2 cliques (each edge is a clique. Trivially, you can specify a graph using its edges)
The second line describes the first clique between vertex 1 and 2. “0” is the delimiter which specifies end of line.
The third line describes the second clique between vertex 2 and vertex 3.

**Options**: 
[option1] should be one of the following:
1. --random-ordering: Branching performed in quickbb using random ordering
2. --min-fill-ordering: Branching performed in quickbb using min-fill ordering

[option2] can be any of the following or combination thereof:
1. --time [int]: Integer time-bound.
2. --lb: Computes lb of the graph,
3. --outfile [filename]: Store the output like treewidth, lb and the current best perfect elimination order (peo).
4. --statfile [filename]: Program statistics. The statfile has the following format:
    * row1:#vertices,
    * row2: #edges
    * row3: lb if --lb is used else 0,
    * row4: an upper bound on the treewidth
    * row5: Time elapsed,
    * row6:Nodes Visited,
    * row7:Nodes Pruned
    * row8:1 if the treewidth given in row3 is exact and 0 if it is not.


### Example
```
$ /quickbb_64 –min-fill-ordering –lb –outfile myout –statfile mystat –cnffile ls8_normalized.cnf
```
This will run quickbb on the ls8_normalized.cnf instance. The ordering heuristic used is min-fill. The output will be stored in myout and the statistics will be stored in mystat
