#ifndef common_h   /* Include guard */
#define common_h

void read_graph_from_file(char *filename, int *N, int **row_ptr, int **col_idx,
   double **val, int **dangling_idx);
void PageRank_iterations(int N, int *row_ptr, int *col_idx, double *val,
  double d, double epsilon, double *scores, int *dangling_idx);
void top_n_webpages(int N, double *scores, int n);
void PageRank_iterations_omp(int N, int *row_ptr, int *col_idx, double *val,
  double d, double epsilon, double *scores, int *dangling_idx);
void top_n_webpages_omp(int N, double *scores, int n);

int max_idx(double arr[], int n);
double largest(double arr[], int N);
void swap(int *a, int *b);
void sort_perm(int *arr, int beg, int end);
void swapper(int* xp, int* yp);
void Sort(int arr[], int arr_2[], int N);

#endif
