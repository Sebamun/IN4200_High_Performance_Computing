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
bool overlap_above(const int rank)
{
  return (rank != 0);
}


int main(int argc, char *argv[]){
      int m, n, c, iters;
      int my_m, my_n, my_rank, num_procs, new_my_m ;
      float kappa;
      image u, u_bar, whole_image; // whole_image is where we want to gather all
      // the contributions in the end.
      unsigned char *image_chars, *my_image_chars, *new_my_image_chars; // kan være at jeg må flytte denne inni loopen.
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
       //allocate_image(&whole_image, m, n);
       //my_image_chars = malloc(m*n * sizeof(my_image_chars));
      }
      // We now broadcast the information from the "root" to all remainig
      // processes:
      MPI_Bcast (&iters, 1, MPI_INT, 0, MPI_COMM_WORLD);
      MPI_Bcast (&kappa, 1, MPI_INT, 0, MPI_COMM_WORLD);
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
          printf("%d \n", sendcounts[rank]);
        }
        for (int rank = 1; rank < num_procs; rank++){

          displs[rank] = displs[rank-1] + sendcounts[rank - 1];
          //displs[rank] = displs[rank] - n;
          //if (rank != (num_procs-1)) displs[rank] = displs[rank] - n;
          //if (rank != 0) displs[rank] = displs[rank] - n;
          //if (rank-1 != (num_procs-1)) displs[rank] = displs[rank] - n;
        }
      }

      my_m = calc_m(m, num_procs, my_rank); // Have to put this after
      // last section as it would be overwritten in the first processor if not.

      my_image_chars = malloc(my_m*n * sizeof (my_image_chars));
      //printf("%d \n", my_m*n);
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
      iso_diffusion_denoising_parallel (&u, &u_bar, kappa, iters, my_rank, num_procs);

      convert_image_to_jpeg(&u_bar, my_image_chars);
      //COMMENT: For imorgen- prøv å omstokke på send og receive. + sjekk convert_jpeg_to_image
      // i tilfellet jeg er nødt til å sette start stop betingelser + offset inni my_image_chars

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
         for (int i = 0; i<new_my_m*n;i++) new_my_image_chars[i] = my_image_chars[i];
       }
      if (my_rank != 0){
        for (int i = 0; i<new_my_m*n;i++) new_my_image_chars[i] = my_image_chars[i + n];
      }


       //convert_image_to_jpeg(&u_bar, my_image_chars);
       MPI_Gatherv(new_my_image_chars, new_my_m*n, MPI_UNSIGNED_CHAR, image_chars, sendcounts, displs, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);

       //MPI_Gatherv(&(my_image_chars[overlap_above(my_rank)*n]), new_my_m*n, MPI_UNSIGNED_CHAR, image_chars, sendcounts, displs, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
       //printf("hei\n");

         /* each process sends its resulting content of u_bar to process 0 */
         /* process 0 receives from each process incoming values and */
         /* copy them into the designated region of struct whole_image */
         /*  ...  */
        //convert_image_to_jpeg(&u_bar, my_image_chars);
        //MPI_Gatherv(my_image_chars, n_rows[my_rank]*n, MPI_UNSIGNED_CHAR ,image_chars ,sendcounts ,Gdispls ,MPI_UNSIGNED_CHAR ,0 ,MPI_COMM_WORLD);

        if (my_rank==0) {
          //convert_image_to_jpeg(&whole_image, image_chars);
          export_JPEG_file(output_jpeg_filename, image_chars, m, n, c, 75);
           //deallocate_image (&whole_image);
        }
        deallocate_image (&u);
        deallocate_image (&u_bar);



        MPI_Finalize ();
















      //int rows = m/num_procs; // The number of rows for each process.
      //int remainder = m%num_procs; // Get value if not divisible by num_procs.
      // We can now collect the m and n values for each process:



      //my_m = my_m + ((my_rank >= (num_procs - remainder)) ? 1:0);
      //my_n = n;

      // Allocate memory for arrays to be used in scatter and gather functions:
      //int *n_rows = malloc(num_procs * sizeof(int)); // Store number of rows for each process.
      //int *sendcounts = (int*) malloc(num_procs * sizeof(int));
      //int *displs = (int*) malloc(num_procs * sizeof(int));
      //int *Gdispls = malloc(num_procs * sizeof *Gdispls);

      /*
      displs[0] = 0;
      Gdispls[0] = 0;

      n_rows[0] = rows + ((0 >= (num_procs - remainder)) ? 1:0) + 1;
      sendcounts[0] = n_rows[0]*n;
      displs[1] = displs[0] + sendcounts[0] - ((n_rows[0]-rows)*n);


      for (int rank = 1; rank<num_procs-1; rank++){
        n_rows[rank] = rows + ((rank >= (num_procs - remainder)) ? 1:0) + 2; // Fill number of rows for each process.
        sendcounts[rank] = n_rows[rank]*n; // Fill number of values for each process.
        displs[rank+1] = displs[rank] + sendcounts[rank] - ((n_rows[rank]-rows)*n); // Starting point for each process.
        Gdispls[rank+1] = Gdispls[rank] + n_rows[rank];
      }

      // Need to take care of last row and number of values:
      n_rows[num_procs-1] = rows + ((num_procs-1) >= (num_procs - remainder) ? 1:0) + 1;
      sendcounts[num_procs-1] = n_rows[num_procs-1]*n;
      displs[num_procs-1] = displs[num_procs-1] + sendcounts[num_procs-1] - ((n_rows[num_procs-1]-rows)*n);
      printf("%d \n", n_rows[num_procs-1]-rows);

      */

      // Allocation of u and u_bar:
      //allocate_image (&u, n_rows[my_rank], my_n);
      //allocate_image (&u_bar, n_rows[my_rank], my_n);


/* This was wrong:
      if (my_rank == 0) {
        //my_image_chars = malloc(m*n * sizeof *my_image_chars);
        my_image_chars = malloc(n_rows[my_rank]*n * sizeof *my_image_chars);
      }
      else my_image_chars = malloc(n_rows[my_rank]*n * sizeof *my_image_chars);
      */

      //my_image_chars = malloc(n_rows[my_rank]*n * sizeof (my_image_chars));

/*
      MPI_Scatterv(image_chars,
        sendcounts,
        displs,
        MPI_UNSIGNED_CHAR,
        my_image_chars,
        n_rows[my_rank]*n,
        MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
  printf("hei \n");
*/


      //my_image_chars = (unsigned char*) malloc(my_m*my_n * sizeof(unsigned char));

      //MPI_Gather(&size, 1, MPI_INT, sendcounts, 1, MPI_INT, 0, MPI_COMM_WORLD);

      //for (int i = 0; i<num_procs; i++){
        //displs[i] = sendcounts[i]*i;
        //printf("%d \n", displs[i]);
      //}
      //printf("%d \n", displs[0]);
      //allocate_image (&u, my_m, my_n);
      //allocate_image (&u_bar, my_m, my_n);
      //each process asks process 0 for a partitioned region
      //of image_chars and copy the values into u
      //MPI_Scatterv(image_chars, sendcounts, displs, MPI_UNSIGNED_CHAR, my_image_chars, my_m*my_n, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);

      //convert_jpeg_to_image (my_image_chars, &u);

      //iso_diffusion_denoising_parallel (&u, &u_bar, kappa, iters, my_rank, num_procs);

       /* each process sends its resulting content of u_bar to process 0 */
       /* process 0 receives from each process incoming values and */
       /* copy them into the designated region of struct whole_image */
       /*  ...  */
      //convert_image_to_jpeg(&u_bar, my_image_chars);
      //MPI_Gatherv(my_image_chars, n_rows[my_rank]*n, MPI_UNSIGNED_CHAR ,image_chars ,sendcounts ,Gdispls ,MPI_UNSIGNED_CHAR ,0 ,MPI_COMM_WORLD);

      //if (my_rank==0) {
      //convert_image_to_jpeg(&whole_image, image_chars);
      //   export_JPEG_file(output_jpeg_filename, image_chars, m, n, c, 75);
      //   deallocate_image (&whole_image);
      //}
      //deallocate_image (&u);
      //deallocate_image (&u_bar);

return 0; }
