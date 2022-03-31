#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "common.h"

int main(int narg, char **argv){
  if (narg<2){
    printf("insert filename");
    exit(0);
  }
  int n; // The number of nodes.
  int *row_ptr, *col_idx; // Row and collumn pointer.
  double *val;

  double epsilon = 0.000001;
  double d = 1.0;
  double *scores;

  read_graph_from_file(argv[1], &n, &row_ptr, &col_idx, &val);
  printf("------------------ \n");
  PageRank_iterations(n, row_ptr, col_idx, val, d, epsilon, scores);

  free(row_ptr); // Free the memory after use.
  free(col_idx);
  free(val);


}
