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
    double *diff;
    diff = (double*) malloc(n * sizeof(double));
    double crit = 1.0;
    int k=0; // How many iterations we need before convergence.
    double term; // Here we store the vector matrix multiplication.
    //int ones[n];
    scores = (double*) malloc(n * sizeof(double));
    // Initialize:
    for (int i = 0; i<n; i++){
      x[i]=1.0/n; // Initial "guess".
    }

    while (crit>epsilon) {
      k++; // Add to counter.
      for (int i=0; i<n; i++){
        term = 0.; // Reset vector matrix multiplication.
        for (int j=row_ptr[i]; j<row_ptr[i+1]; j++){
          term = term + val[j] * x[col_idx[j]]; // Sparse matrix multiplication.
        }
        scores[i] =  d*term; // The PageRank algorithm.
        // Our stopping criteria is given by the difference between x[i]
        // and scores[i]:
        diff[i] = fabs(scores[i] - x[i]);
        crit = largest(diff, n); // Find the largest difference.
        }
        // We now update our x[i] according to the new calculated scores[i].
        // (Note that this has to be done iteratively, as just
        // putting the pointers equal to each other dosent work.)
        for (int i =0; i<n; i++) x[i] = scores[i];
      }
      for (int i=0; i < n; i++) printf("%f \n", scores[i]); // Converged score.
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
