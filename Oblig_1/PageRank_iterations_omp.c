#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <malloc/malloc.h>
#include <omp.h>

#include "common.h"

void PageRank_iterations_omp(int N, int *row_ptr, int *col_idx, double *val,
  double d, double epsilon, double *scores, int *dangling_idx){

  int i;
  int j;

  double *x; // Vector of numerical values.
  x = (double*) malloc(N * sizeof(double));
  double *diff; // Difference between previous x and the scores for that
  // iteration.
  diff = (double*) malloc(N * sizeof(double));
  double crit = 1.0; // The critical condition, which is updated for each
  // while loop iteration.
  int k=0; // How many while loop iterations we need before convergence.
  double term; // Here we store the vector matrix multiplication.
  double dangling;
  // Initialize:
  for (i = 0; i<N; i++){
    x[i]=1.0/N; // Initial "guess".
  }

  while (crit>epsilon) { // Criteria for updating scores.
    k++; // Add to counter.

    #pragma omp parallel for reduction(+:term) private(j)
    {
    for (i=0; i<N; i++){
      term = 0.; // Reset vector matrix multiplication.
      dangling = 0.; // Reset dangling term.
      if (dangling_idx[i]==1){
      dangling = (1 - d + ( d * x[col_idx[row_ptr[i]]] ) ) / N;
      }
        for (j=row_ptr[i]; j<row_ptr[i+1]; j++){
          term = term + val[j] * x[col_idx[j]]; // Sparse matrix multiplication.
        }

      scores[i] = dangling + d*term; // The PageRank algorithm.
      // Our stopping criteria is given by the difference between x[i]
      // and scores[i]:
      diff[i] = fabs(scores[i] - x[i]);
      crit = largest(diff, N); // Find the largest difference.
      }
    }
      // We now update our x[i] according to the new calculated scores[i].
      // (Note that this has to be done iteratively, as just
      // setting the pointers equal to each other dosent work.)
      for (i =0; i<N; i++) x[i] = scores[i];
    }
    printf("Converged score: \n");
    for (i=0; i < N; i++) printf("%f \n", scores[i]); // Converged score.
  }
