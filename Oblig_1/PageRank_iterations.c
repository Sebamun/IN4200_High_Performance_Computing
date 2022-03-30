#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <malloc/malloc.h>

#include "common.h"

void PageRank_iterations(int n, int *row_ptr, int *col_idx, double *val,
  double d, double epsilon, double *scores){

    printf("------------------ \n");
    double *x;
    x = (double*) malloc(n * sizeof(double));
    double *diff;
    diff = (double*) malloc(n * sizeof(double));
    double crit = 1.0;

    double term;
    double x_upd[n];
    int ones[n];
    scores = (double*) malloc(n * sizeof(double));
    // Initialize:
    //term[0] = 0;
    for (int i = 0; i<n; i++){
      x[i]=1.0/n;
      ones[i] = 1.0;
    }

    while (crit>epsilon) {
      /* code */
      for (int i=0; i<n; i++){
        term = 0;
        for (int j=row_ptr[i]; j<row_ptr[i+1]; j++){
          term += val[j] * x[col_idx[j]]; // Sparse matrix multiplication.
        }
        scores[i] = term;
        printf("%f \n", scores[i]);
        diff[i] = fabs(scores[i] - x[i]);
        //printf("%f", diff[i])
      }
      crit = largest(diff, n);
      //printf("%f \n", crit);
      x = scores;
      printf("---- \n");
  }

}
