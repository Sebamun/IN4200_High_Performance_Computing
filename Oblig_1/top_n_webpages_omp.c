#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <malloc/malloc.h>
#include <omp.h>

#include "common.h"

void top_n_webpages_omp(int N, double *scores, int n){
  int i;
  double *top_n_scores; // Here we store the score for top n webpages.
  top_n_scores = (double*) malloc(n * sizeof(double));
  int *top_n_indicies; // Here we score the indicies for the top n webpages.
  top_n_indicies = (int*) malloc(n * sizeof(int));
  // We want to find the highest computed score n
  // times and remove the highest score before the next iteration:
  for (int i = 0; i < n; i++){
    int j;
    int max_idx;
    double max; // max value for each iteration.
    max = scores[0]; // This is the initial max value each time.
  #pragma omp parallel for
    for (int j = 1; j < N; j++){ // Run through the other score values.
      #pragma omp critical
      if (scores[j]>max){ // Criteria for changing max value/index.
        max_idx = j;
        max = scores[j];
      }
    }
    top_n_scores[i] = max; // Add the max values to array.
    top_n_indicies[i] = max_idx; // Add the corresponding indicies to array.
    scores[max_idx] = 0.0; // "Remove" the max element before next iteration.
  }
  //Printing:
  printf("Webpage   |   Score     |\n");
  for (int i = 0; i<n; i++) printf("%d         |   %f  |\n",top_n_indicies[i],
  top_n_scores[i]);


  // Free the memory after use:
  free(top_n_scores);
  free(top_n_indicies);
}
