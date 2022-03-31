#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <malloc/malloc.h>

#include "common.h"

// In this function we want to go through the vector scores and find
// the top n highest webpages.
void top_n_webpages(int N, double *scores, int n){
  double *top_n_scores;
  top_n_scores = (double*) malloc(n * sizeof(double));
  int *top_n_indicies;
  top_n_indicies = (int*) malloc(n * sizeof(int));
  // We want to find the highest computed score n
  // times and remove the highest score before the next iteration:
  for (int i = 0; i < n; i++){
    int max_idx;
    int j;
    double max;
    max = scores[0];
    for (int j = 1; j < N; j++){
      if (scores[j]>max){
        max_idx = j;
        max = scores[j];
      }
    }
    top_n_scores[i] = max;
    top_n_indicies[i] = max_idx;
    scores[max_idx] = 0.0; // "Remove" the max element before next iteration.
  }
  for (int i = 0; i<n; i++) printf("%f \n", top_n_scores[i]);
  printf("------------\n");
  for (int i = 0; i<n; i++) printf("%d \n", top_n_indicies[i]);

}
