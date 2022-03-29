#ifndef common_h   /* Include guard */
#define common_h

void read_graph_from_file(char *filename, int *n, int **row_ptr, int **col_idx,
   double **val);
void PageRank_iterations(int n, int *row_ptr, int *col_idx, double *val,
  double d, double epsilon, double *scores);

void swap(int *a, int *b);
void sort_perm(int *arr, int beg, int end);
void swapper(int* xp, int* yp);
void Sort(int arr[], int arr_2[], int n);

#endif
