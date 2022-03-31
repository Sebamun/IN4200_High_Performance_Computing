#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <malloc/malloc.h>

#include "common.h"

void PageRank_iterations(int n, int *row_ptr, int *col_idx, double *val,
  double d, double epsilon, double *scores, int *dangling_idx){

    scores = (double*) malloc(n * sizeof(double)); // Allocate memory for ptr.
    double *x; // Vector of numerical values.
    x = (double*) malloc(n * sizeof(double));
    double *diff; // Difference between previous x and the scores for that
    // iteration.
    diff = (double*) malloc(n * sizeof(double));
    double crit = 1.0; // The critical condition, which is updated for each
    // while loop iteration.
    int k=0; // How many while loop iterations we need before convergence.
    double term; // Here we store the vector matrix multiplication.
    double dangling;
    // Initialize:
    for (int i = 0; i<n; i++){
      x[i]=1.0/n; // Initial "guess".
    }

    while (crit>epsilon) { // Criteria for updating scores.
      k++; // Add to counter.
      for (int i=0; i<n; i++){
        term = 0.; // Reset vector matrix multiplication.
        dangling = 0.; // Reset dangling term.
        if (dangling_idx[i]==1){
        dangling = (1 - d + ( d * x[col_idx[row_ptr[i]]] ) ) / n;
        }

        for (int j=row_ptr[i]; j<row_ptr[i+1]; j++){
          term = term + val[j] * x[col_idx[j]]; // Sparse matrix multiplication.
        }
        //printf("%f", dangling);
        scores[i] = dangling + d*term; // The PageRank algorithm.
        //printf("%f \n", dangling);
        // Our stopping criteria is given by the difference between x[i]
        // and scores[i]:
        diff[i] = fabs(scores[i] - x[i]);
        crit = largest(diff, n); // Find the largest difference.
        }
        // We now update our x[i] according to the new calculated scores[i].
        // (Note that this has to be done iteratively, as just
        // setting the pointers equal to each other dosent work.)
        for (int i =0; i<n; i++) x[i] = scores[i];
      }
      for (int i=0; i < n; i++) printf("%f \n", scores[i]); // Converged score.
    }
