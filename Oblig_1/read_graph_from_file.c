#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <malloc/malloc.h>

#include "common.h"

void read_graph_from_file(char *filename, int *n, int **row_ptr, int **col_idx,
    double **val, int **dangling_idx){
  int i; // Used for indexing.
  int edges; // This is the number of lines of data.
  FILE *datafile; // Here we declare pointer to file.

  datafile = fopen(filename, "r");

  if (datafile == NULL) { // Abort if file is missing when calling exe.
    printf("Failed to open file %s. \n", filename);
    exit(0);
  }

  datafile = fopen(filename, "r");
  fscanf(datafile, "%*[^\n]\n"); // Skip line
  fscanf(datafile, "%*[^\n]\n"); // Skip line
  // We can now collect the number of nodes and edges:
  fscanf(datafile, "# Nodes: %d Edges: %d \n", n, &edges);
  fscanf(datafile, "%*[^\n]\n"); // Skip line
  int counter[*n]; // Use this to store the number of elements in each col.
  //int outbound[edges]; // Define array where we store outbound values.
  //int inbound[edges]; // Define array where we store inbound values.
  int *outbound;
  int *inbound;
  outbound = (int*)malloc(edges * sizeof(int));
  inbound = (int*)malloc(edges * sizeof(int));
  int in;
  int out;
  int self_links = 0;
  for (i = 0; i<*n; i++){ // Initialize the counter with zeros.
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
      counter[outbound[i]]++; // This will be used in val array.
    }
    else {
      i--; // This "removes" the self links.
      self_links++; // add to the self_link counter.
    }
    //printf("%d %d \n", outbound[i], inbound[i]);
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

  *row_ptr = (int*) malloc((*n+1) * sizeof(int)); // Allocate memory for the
  // row pointer.
  (*row_ptr)[0] = 0; // The first element in the row pointer will always be 0
  // as that is how the first element in the row pointer is defined.
  for (int i = 0; i<edges; i++){
    int idx = inbound[i]; // define index for row_ptr given number value in
    // inbound array.
    (*row_ptr)[idx+1] = i+1; // Give values to row_ptr.
  }

  *col_idx = (int*) malloc(edges * sizeof(int)); // Allocate memory for the
  // col_idx array.
  // We now want to assign values to our col_idx vector. This is done by
  // reordering sections of the outbound array:
  for (int i=0; i<*n; i++){
    int start = (*row_ptr)[i], stop = (*row_ptr)[i+1];
    Sort( &(outbound[start]), &(inbound[start]), stop-start);
    // We want to set the zero elements in the row_ptr equal to the previous
    // value:
    if ((*row_ptr)[i+1]==0) (*row_ptr)[i+1] = (*row_ptr)[i];
    }

  *val = (double*) malloc(edges * sizeof(double)); // Allocate memory for the
  // val array. Finally we need to give values to this array;
  for (int i = 0; i < edges; i ++) {
    (*col_idx)[i] = outbound[i]; // Fill col_idx with outbound.
    (*val)[i] = 1.0 / counter[ (*col_idx)[i] ];
  }
  free(inbound);
  free(outbound);
  // We also want to know the indexes of the dangling wepages:
  *dangling_idx = (int*) malloc(*n * sizeof(int));
  for (int i = 0; i<*n; i++){
    if (counter[i]==0) (*dangling_idx)[i] = 1;
  }
  // To check the row_ptr, col_idx and val:
  /*
  printf("row_ptr: \n");
  for (int i=0; i<*n+1; i++){
  printf("%d \n", (*row_ptr)[i] );
  }

  printf("\n");
  printf("col_idx: val: \n");
  for (int i=0; i<edges; i++){
  printf("%d        %f \n", (*col_idx)[i], (*val)[i]);
  }
  */
}
