#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <malloc/malloc.h>

#include "common.h"

void read_graph_from_file(char *filename, int *n, int **row_ptr, int **col_idx,
    double **val){
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
  int outbound[edges]; // Define array where we store outbound values.
  int inbound[edges]; // Define array where we store inbound values.
  int in;
  int out;
  int self_links = 0;
  // We then assign values to these arrays:
  for (i = 0; i<*n; i++){ // Initialize the counter with zeros.
    counter[i] = 0;
  }
  for (i = 0; i<edges; i++){

    // Her må jeg huske å legge til en if test for self links.

    fscanf(datafile, "%d %d \n", &out, &in);
    //if (out != in){

    //if (in == i && in != out){
      outbound[i]=out;
      inbound[i]=in;

    //if (ToNodeId[j] == i && ToNodeId[j] != FromNodeId[j])
    counter[outbound[i]]++; // This will be used in val array.
    //}
    //else{
    //  self_links++;
    //}
  //}
    printf("%d %d \n", outbound[i], inbound[i]);
  }

  fclose(datafile); // Close the file.
  // We now want to sort our inbound array. We will use the quicksort algorithm,
  // such that we can order our numbers in ascending order for the inbound
  // array. We also want our oubound array to follow the changes in the inbound
  // array:

  Sort(inbound, outbound, edges);

  *row_ptr = (int*) malloc((*n+1) * sizeof(int)); // Allocate memory for the
  // row pointer. Note: In the textbook they defined this array with length
  // n+1, we decided to use length=n instead.
  (*row_ptr)[0] = 0; // The first element in the row pointer will always be 0
  // as that is how the first element in the row pointer is defined.
  for (int i = 0; i<edges; i++){
    int idx = inbound[i]; // define index for row_ptr given number value in
    // inbound array.
    (*row_ptr)[idx+1] = i+1; // Give values to row_ptr.
  }
  for (int i = 0; i<*n; i++){
    if ((*row_ptr)[i+1]==0) (*row_ptr)[i+1] = (*row_ptr)[i];
  }

  *col_idx = (int*) malloc(edges * sizeof(int)); // Allocate memory for the
  // col_idx array.
  // We now want to assign values to our col_idx vector. This is done by
  // reordering sections of the outbound array:
  for (int i=0; i<*n; i++){
    int start = (*row_ptr)[i], stop = (*row_ptr)[i+1];
    Sort( &(outbound[start]), &(inbound[start]), stop-start);
    }

  *col_idx = outbound; // Fill col_idx with outbound.
  *val = (double*) malloc(edges * sizeof(double)); // Allocate memory for the
  // val array. Finally we need to give values to this array;
  for (int i = 0; i < edges; i ++) {
    (*val)[i] = 1.0 / counter[ (*col_idx)[i] ];
  }

  printf("row_ptr: \n");
  for (int i=0; i<*n+1; i++){
  printf("%d \n", (*row_ptr)[i] );
  }
  printf("\n");
  printf("col_idx: val: \n");
  for (int i=0; i<edges; i++){
  printf("%d        %f \n", (*col_idx)[i], (*val)[i]);
  }
  }
