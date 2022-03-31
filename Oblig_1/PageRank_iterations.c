#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <malloc/malloc.h>

#include "common.h"

void PageRank_iterations(int n, int *row_ptr, int *col_idx, double *val,
  double d, double epsilon, double *scores){

    double *x;
    x = (double*) malloc(n * sizeof(double));
    //double *diff;
    //diff = (double*) malloc(n * sizeof(double));

    double term;
    //int ones[n];
    scores = (double*) malloc(n * sizeof(double));
    // Initialize:
    for (int i = 0; i<n; i++){
      x[i]=1.0/n;
    }

    for (int m = 0; m < 100; m++){
    //while (crit>epsilon) {
      for (int i=0; i<n; i++){
        term = 0.;
        for (int j=row_ptr[i]; j<row_ptr[i+1]; j++){
          term = term + val[j] * x[col_idx[j]]; // Sparse matrix multiplication.
        }
        scores[i] =  d*term;
        printf("%f\n", scores[i]);
        }
        for (int i =0; i<n; i++) x[i] = scores[i];
        printf("---------\n");
      }
        //for (int i=0; i < n; i++) printf("%f \n", x[i]);
    }





        //printf("%f \n", scores[i]);
        //diff[i] = fabs(scores[i] - x[i]);
        //x[i] = scores[i];

        //printf("%f", diff[i])

      //crit = largest(diff, n);
      //printf("%f \n", crit);
      //x = scores;
      //printf("---- \n");
    //}
  //}
