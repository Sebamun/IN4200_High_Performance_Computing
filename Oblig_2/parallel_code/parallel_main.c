/* needed header files .... */
#include <stdio.h>
#include <mpi.h>
#include "parallel_common.h" // Import struct from this header file.
#include "stdbool.h"


#ifdef __MACH__ // if Macintosh operating system.
#include <stdlib.h>
#else
#include <malloc.h>
#endif
     /* declarations of functions import_JPEG_file and export_JPEG_file */
void import_JPEG_file (const char* filename, unsigned char** image_chars,
                      int* image_height, int* image_width,
                      int* num_components);
void export_JPEG_file (const char* filename, const unsigned char* image_chars,
                      int image_height, int image_width,
                      int num_components, int quality);

int main(int argc, char *argv[]){
      int m, n, c, iters;
      int my_m, my_n, my_rank, num_procs, new_my_m ;
      float kappa;
      image u, u_bar;
      // the contributions in the end.
      unsigned char *image_chars, *my_image_chars, *new_my_image_chars;
      char *input_jpeg_filename, *output_jpeg_filename;
      int *sendcounts, *displs;

      MPI_Init (&argc, &argv);
      MPI_Comm_rank (MPI_COMM_WORLD, &my_rank);
      MPI_Comm_size (MPI_COMM_WORLD, &num_procs);

      input_jpeg_filename = argv[1];
      output_jpeg_filename = argv[2];
      kappa = atof(argv[3]);
      iters = atoi(argv[4]);

      if (my_rank==0) {
       import_JPEG_file(input_jpeg_filename, &image_chars, &m, &n, &c);
      }
      // We now broadcast the information from the "root" to all remainig
      // processes:
      MPI_Bcast (&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
      MPI_Bcast (&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
      /* 2D decomposition of the m x n pixels evenly among the MPI processes */
      if (my_rank == 0){

        sendcounts =  malloc(num_procs * sizeof(int));
        displs =  malloc(num_procs * sizeof(int));
        displs[0] = 0;

        for (int rank = 0; rank<num_procs; rank ++){
          my_m = calc_m(m, num_procs, rank);
          sendcounts[rank] = my_m * n;
        }
        for (int rank = 1; rank < num_procs; rank++){

          displs[rank] = displs[rank-1] + sendcounts[rank - 1];
          displs[rank] = displs[rank] - n;
          if (rank != (num_procs-1)) displs[rank] = displs[rank] - n;
        }
      }

      my_m = calc_m(m, num_procs, my_rank); // Have to put this after
      // last section as it would be overwritten in the first processor if not.

      my_image_chars = malloc(my_m*n * sizeof (my_image_chars));

      MPI_Scatterv(image_chars,
        sendcounts,
        displs,
        MPI_UNSIGNED_CHAR,
        my_image_chars,
        my_m*n,
        MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);

      // Allocation of u and u_bar:
      allocate_image (&u, my_m, n);
      allocate_image (&u_bar, my_m, n);

      convert_jpeg_to_image (my_image_chars, &u);
      iso_diffusion_denoising_parallel (&u, &u_bar, kappa, iters, my_rank
        , num_procs);

      convert_image_to_jpeg(&u_bar, my_image_chars);

       // We now need to adjust our send_counts and displs:

      if (my_rank == 0){
        displs[0] = 0;
        for (int rank = 0; rank<num_procs; rank ++){
          new_my_m = calc_new_m(m, num_procs, rank);
          sendcounts[rank] = new_my_m * n;
        }
        for (int rank = 1; rank < num_procs; rank++)
          displs[rank] = displs[rank-1] + sendcounts[rank-1];
        }

       // We now need to revert back to the original actual number of rows for
       // each section:
       new_my_m = calc_new_m(m, num_procs, my_rank);
       new_my_image_chars = malloc(new_my_m*n * sizeof (my_image_chars));

       if (my_rank == 0){
         for (int i = 0; i<new_my_m*n;i++){
            new_my_image_chars[i] = my_image_chars[i];
         }
       }
      if (my_rank != 0){
        for (int i = 0; i<new_my_m*n;i++){
          new_my_image_chars[i] = my_image_chars[i + n];
        }
      }

       MPI_Gatherv(new_my_image_chars,
         new_my_m*n,
         MPI_UNSIGNED_CHAR,
         image_chars,
         sendcounts,
         displs,
         MPI_UNSIGNED_CHAR,
         0,
         MPI_COMM_WORLD);

        if (my_rank==0) {
          export_JPEG_file(output_jpeg_filename, image_chars, m, n, c, 75);
        }
        deallocate_image (&u);
        deallocate_image (&u_bar);

        MPI_Finalize ();

return 0; }
