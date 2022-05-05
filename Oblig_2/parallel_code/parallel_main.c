/* needed header files .... */
#include <stdio.h>
#include <mpi.h>
#include "parallel_common.h" // Import struct from this header file.

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
      int my_m, my_n, my_rank, num_procs;
      float kappa;
      image u, u_bar, whole_image; // whole_image is where we want to gather all
      // the contributions in the end.
      unsigned char *image_chars, *my_image_chars; // kan være at jeg må flytte denne inni loopen.
      char *input_jpeg_filename, *output_jpeg_filename;

      MPI_Init (&argc, &argv);
      MPI_Comm_rank (MPI_COMM_WORLD, &my_rank);
      MPI_Comm_size (MPI_COMM_WORLD, &num_procs);

      //printf("my rank is %d.\n", my_rank);
      //printf("The number of processes are %d.\n", num_procs);

     input_jpeg_filename = argv[1];
     output_jpeg_filename = argv[2];
     kappa = atof(argv[3]);
     iters = atoi(argv[4]);

      if (my_rank==0) {
       import_JPEG_file(input_jpeg_filename, &image_chars, &m, &n, &c);
       allocate_image(&whole_image, m, n);
      }
      // We now broadcast the information from the "root" to all remainig
      // processes:
      MPI_Bcast (&iters, 1, MPI_INT, 0, MPI_COMM_WORLD);
      MPI_Bcast (&kappa, 1, MPI_INT, 0, MPI_COMM_WORLD);
      MPI_Bcast (&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
      MPI_Bcast (&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
      /* 2D decomposition of the m x n pixels evenly among the MPI processes */
      /*
      int my_start = (my_rank * (m-2)) / num_procs;
      int my_stop = (my_rank + 1) * (m-2) / num_procs;
      my_m = my_stop - my_start + 2;
      my_n = n;
      int chunk = my_m*my_n;
*/
/*
      int my_m_temp = m / num_procs;
      int rest = m % num_procs;

      if(rest != 0) my_m_temp++;

      int my_start = my_rank * my_m_temp;
      int my_stop = (my_rank + 1) * my_m_temp;

      if(my_rank == num_procs - 1) my_stop = m;

      my_m = my_stop - my_start;
      my_n = n;
      int size = my_m*my_n;

      // We now want to construct a array sendcounts, where the elements equals
      // the length of each section we want to distribute to the processes.
*/
      int rows = m/num_procs; // The number of rows for each process.
      // We add two rows which are to be used as ghost point rows.
      int remainder = m%num_procs; // Get value if not divisible by num_procs.
      // We can now collect the m and n values for each process:
      //my_m = my_m + ((my_rank >= (num_procs - remainder)) ? 1:0);
      my_n = n;
      //printf("%d \n", my_n);
      //printf("%d", rows);
      // Allocation of u and u_bar:
      //allocate_image (&u, my_m, my_n);
      //allocate_image (&u_bar, my_m, my_n);
      // Allocate memory for arrays to be used in scatter and gather functions:
      int *n_rows = malloc(num_procs * sizeof(int)); // Store number of rows for each process.
      int *sendcounts = (int*) malloc(num_procs * sizeof(int));
      int *displs = (int*) malloc(num_procs * sizeof(int));
      int *Gdispls = malloc(num_procs * sizeof *Gdispls);
      displs[0] = 0;
      Gdispls[0] = 0;

      n_rows[0] = rows + ((0 >= (num_procs - remainder)) ? 1:0) + 1;
      sendcounts[0] = n_rows[0]*n;
      displs[1] = displs[0] + sendcounts[0] ;


      for (int rank = 1; rank<num_procs-1; rank++){
        n_rows[rank] = rows + ((rank >= (num_procs - remainder)) ? 1:0) + 2; // Fill number of rows for each process.
        sendcounts[rank] = n_rows[rank]*n; // Fill number of values for each process.
        displs[rank+1] = displs[rank] + sendcounts[rank]; // Starting point for each process.

        Gdispls[rank+1] = Gdispls[rank] + n_rows[rank];
      }


      // Need to take care of last row and number of values:
      n_rows[num_procs-1] = rows + ((num_procs-1) >= (num_procs - remainder) ? 1:0) + 1;
      sendcounts[num_procs-1] = n_rows[num_procs-1]*n;

      // Comment til imorgen: forsett å print verdier for de ulike parametrene
      //scatter funksjon.

      //displs[num_procs-1] = displs[rank] + sendcounts[rank]-2;
      //for (int rank = 1; rank<num_procs-1; rank++){
      //  n_rows[rank] = n_rows[rank] + 2;
      //}
      // Allocation of u and u_bar:
      allocate_image (&u, n_rows[my_rank], my_n);
      allocate_image (&u_bar, n_rows[my_rank], my_n);


/* This was wrong:
      if (my_rank == 0) {
        //my_image_chars = malloc(m*n * sizeof *my_image_chars);
        my_image_chars = malloc(n_rows[my_rank]*n * sizeof *my_image_chars);
      }
      else my_image_chars = malloc(n_rows[my_rank]*n * sizeof *my_image_chars);
      */

      my_image_chars = malloc(n_rows[my_rank]*n * sizeof *my_image_chars);


      MPI_Scatterv(image_chars,
        sendcounts,
        displs,
        MPI_UNSIGNED_CHAR,
        my_image_chars,
        n_rows[my_rank]*n,
        MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
  printf("hei \n");



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

      MPI_Finalize ();
return 0; }
