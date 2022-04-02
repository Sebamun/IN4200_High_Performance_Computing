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
  int N; // The number of nodes.
  int *row_ptr, *col_idx; // Row and collumn pointer.
  double *val;
  int *dangling_idx; // Reason for defining this pointer here:
  // found it easier to collect the dangling webpages in the readfile function
  // than in the PageRank function.

  double epsilon = 0.000001; // Stopping criteria.
  double d = 1.0; // Damping constant.
  double *scores; // PageRank score vector.

  int n = 5; // Top n webpages.

  read_graph_from_file(argv[1], &N, &row_ptr, &col_idx, &val, &dangling_idx);
  printf("------------------- \n");
  /*
  // We can use the N to allocate memory to the scores vector:
  scores = (double*) malloc(N * sizeof(double));
  PageRank_iterations(N, row_ptr, col_idx, val, d, epsilon, scores,
    dangling_idx);
  printf("------------------------ \n");
  top_n_webpages(N, scores, n);
  */
  scores = (double*) malloc(N * sizeof(double));
  PageRank_iterations_omp(N, row_ptr, col_idx, val, d, epsilon, scores,
    dangling_idx);
  printf("------------------------ \n");
  top_n_webpages_omp(N, scores, n);

  // Free the memory after use:
  free(row_ptr);
  free(col_idx);
  free(val);
  free(scores);
  free(dangling_idx);

}
