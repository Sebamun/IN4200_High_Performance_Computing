#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <malloc/malloc.h>

#include "common.h"

void PageRank_iterations(int n, int *row_ptr, int *col_idx, double *val,
  double d, double epsilon, double *scores){

    printf("------------------ \n");
    int x[n];
    int term[n];
    // Initialize:
    for (int i = 0; i<n; i++){
      x[i]=1;
      term[i] = 0;
    }
    // We are going to perform a sparse matrix-vector multiplication:
    for (int i=1; i<n; i++){
      for (int j=row_ptr[i]; j<row_ptr[i+1]-1; j++){
        term[i] = term[i] + val[j] * x[col_idx[i]];
      }
    }
    for (int i = 0; i<n; i++) printf("%d \n", term[i]);
  }
