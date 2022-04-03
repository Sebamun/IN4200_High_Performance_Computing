# IN4200 Mandatory assignment 1 "Computing PageRank"
This is the first mandatory assignment in the course High-Performance Computing and Numerical Projects.
The project revolves around reading a textfile containing information over how different webpages links to eachother.
This can be used to calculate the score which is a measure of the "importance" of a specific webpage, which depends on
the number of webpages that link to it and the importance of these websites.

## The project used the following coding language and packages:
* C
  * stdlib.h
  * stdio.h
  * math.h
  * string.h
  * time.h
  * malloc/malloc.h
  * omp.h

## How to run the code:
Open terminal window, these commands compile and execute the programs:
```
# To compile all of the files:
make all
# Run program for smallest textfile:
./run_1 simple_webgraph.txt 1.0 0.000001 5
```
Where filename = simple_webgraph.txt, d = 1.0, epsilon = 0.000001 and n = 5. You can changes these values as you please. 
The compilation arguments is given in the makefile, which looks like this: 

```
all: compile

compile:
	gcc read_graph_from_file.c PageRank_iterations.c top_n_webpages.c \
	top_n_webpages_omp.c PageRank_iterations_omp.c functions.c main.c \
	-o run_1 -Xpreprocessor -fopenmp -lomp
```


The folder IN4200_Oblig1_xxx contains the following files:

* read_graph_from_file.c
* PageRank_iterations.c
* PageRank_iterations_omp.c)
* top_n_webpages.c 
* file_top_n_webpages_omp.c
* main.c
* README.md
* common.h
* functions.c
* makefile

For the simple textfile we have the following ouptut:
```
--------------------
row_ptr: 
0 
1 
4 
5 
6 
9 
12 
14 
17 
--------------------
col_idx: | val: 
6        | 0.333333 
0        | 0.500000 
2        | 0.500000 
3        | 0.333333 
0        | 0.500000 
1        | 1.000000 
2        | 0.500000 
3        | 0.333333 
6        | 0.333333 
3        | 0.333333 
4        | 0.333333 
7        | 0.500000 
4        | 0.333333 
7        | 0.500000 
4        | 0.333333 
5        | 1.000000 
6        | 0.333333 
------------------- 
Converged score: 
0.060000 
0.067500 
0.030000 
0.067500 
0.097500 
0.202500 
0.180000 
0.295000 
------------------------ 
Webpage   |   Score     |
7         |   0.295000  |
5         |   0.202500  |
6         |   0.180000  |
4         |   0.097500  |
3         |   0.067500  |
------------------------ 
After parallelisation:
------------------------ 
Converged score: 
0.060000 
0.067500 
0.030000 
0.067500 
0.097500 
0.202500 
0.180000 
0.295000 
------------------------ 
Webpage   |   Score     |
7         |   0.295000  |
5         |   0.202500  |
6         |   0.180000  |
4         |   0.097500  |
3         |   0.067500  |
```

Comment: found that for larger files the "read_graph_from_file" function is really slow, which is most likely due to the sorting 
algorithm I used. It would therefore be interesting to implement a more efficient sorting algorithm, 
This is one of the aspects whcih can be improved in my code. 

