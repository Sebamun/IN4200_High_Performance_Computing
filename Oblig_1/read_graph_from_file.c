#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <malloc/malloc.h>

#include "common.h"

void read_graph_from_file(char *filename, int *N, int **row_ptr, int **col_idx,
    double **val, int **dangling_idx){
  int i; // Used for indexing.
  int edges; // This is the number of lines of data.
  FILE *datafile; // Here we declare pointer to file.

  datafile = fopen(filename, "r");

  if (datafile == NULL) { // Abort if file is missing when calling exe.
    printf("Failed to open file %s. \n", filename);
    exit(0);
  }

  datafile = fopen(filename, "r"); // Open file.
  fscanf(datafile, "%*[^\n]\n"); // Skip line
  fscanf(datafile, "%*[^\n]\n"); // Skip line
  // We can now collect the number of nodes and edges:
  fscanf(datafile, "# Nodes: %d Edges: %d \n", N, &edges);
  fscanf(datafile, "%*[^\n]\n"); // Skip line
  int counter[*N]; // Use this to store the number of elements in each col.
  int *outbound; // Define pointer where we store outbound values.
  int *inbound; // Define pointer where we store inbound values.
  outbound = (int*)malloc(edges * sizeof(int));
  inbound = (int*)malloc(edges * sizeof(int));
  int in; // In and out are used to check if we have any self links.
  int out;
  int self_links = 0; // Here we store the number of self links.
  for (i = 0; i<*N; i++){ // Initialize the counter with zeros.
    counter[i] = 0;
  }
  // We then assign values to these arrays:
  for (i = 0; i<edges; i++){

    fscanf(datafile, "%d %d \n", &out, &in);
    // We perform an if test in case we find self links, which are to be
    // excluded:
    if (out != in){
      outbound[i]=out;
      inbound[i]=in;
      counter[outbound[i]]++; // This will be used in val array
      // and for dangling webpages.
    }
    else {
      i--; // This "removes" the self links.
      self_links++; // Add to the self_link counter.
    }
  }
  fclose(datafile); // Close the file.

  // If we have self links we need to reallocate the outbound and inbound
  // array to adjust for the new length:
  outbound = (int*)realloc(outbound, (edges-self_links)*sizeof(int));
  inbound = (int*)realloc(inbound, (edges-self_links)*sizeof(int));

  // We now want to sort our inbound array. We want to order our numbers in
  // ascending order.
  // We also want our outbound array to follow the changes in the inbound
  // array:
  Sort(inbound, outbound, edges);

  *row_ptr = (int*) malloc((*N+1) * sizeof(int)); // Allocate memory for the
  // row pointer.
  (*row_ptr)[0] = 0; // The first element in the row pointer will always be 0
  // as that is how the first element in the row pointer is defined.
  for (i = 0; i<edges; i++){
    int idx = inbound[i]; // define index for row_ptr given number value in
    // inbound array.
    (*row_ptr)[idx+1] = i+1; // Give values to row_ptr.
  }

  *col_idx = (int*) malloc(edges * sizeof(int)); // Allocate memory for the
  // col_idx array.
  // We now want to assign values to our col_idx vector. This is done by
  // reordering sections of the outbound array:
  for (i=0; i<*N; i++){
    int start = (*row_ptr)[i], stop = (*row_ptr)[i+1];
    Sort( &(outbound[start]), &(inbound[start]), stop-start);
    // We want to set the zero elements in the row_ptr equal to the previous
    // value (not the first element):
    if ((*row_ptr)[i+1]==0) (*row_ptr)[i+1] = (*row_ptr)[i];
    }

  *val = (double*) malloc(edges * sizeof(double)); // Allocate memory for the
  // val array. Finally we need to give values to this array;
  for (int i = 0; i < edges; i ++) {
    (*col_idx)[i] = outbound[i]; // Fill col_idx with outbound.
    (*val)[i] = 1.0 / counter[ (*col_idx)[i] ]; // Calculate val for each i.
  }
  free(inbound); // Free memory of pointers declared in this file.
  free(outbound);
  // We also want to know the indexes of the dangling wepages:
  *dangling_idx = (int*) malloc(*N * sizeof(int));
  for (i = 0; i<*N; i++){
    if (counter[i]==0) (*dangling_idx)[i] = 1;
  }
  // To check the row_ptr, col_idx and val:
  printf("--------------------\n");
  printf("row_ptr: \n");
  for (i=0; i<*N+1; i++){
  printf("%d \n", (*row_ptr)[i] );
  }

  printf("--------------------\n");
  printf("col_idx: | val: \n");
  for (i=0; i<edges; i++){
  printf("%d        | %f \n", (*col_idx)[i], (*val)[i]);
  }
}
