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
# Run program:
./run_1 filename
```


Comment: found that for larger files the "read_graph_from_file" function is really slow, which is most likely due to the sorting 
algorithm I used. It would therefore be interesting to implement a more efficient sorting algorithm, 
This is one of the aspects whcih can be improved in my code. 

