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

  read_graph_from_file(argv[1], &n, &row_ptr, &col_idx, &val);

  //free(row_ptr); // Free the memeoty after use.
  //free(col_idx); // Get error when using this function.
  //free(val);


}
