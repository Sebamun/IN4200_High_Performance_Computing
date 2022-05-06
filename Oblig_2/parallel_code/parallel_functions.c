#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#include "parallel_common.h" // Import struct from this header file.

int calc_m(int m, int num_procs, int rank){
  int my_start, my_stop;
  int term = m / num_procs;

  if (rank != 0){
    my_start = term * rank - 1;
  }
  else{
    my_start = 0;
  }
  if (rank != num_procs-1){
    my_stop = term * (rank + 1) + 1;
  }
  else{
    my_stop = m;
  }
  int my_m = my_stop - my_start;
  return my_m;
}

int calc_new_m(int m, int num_procs, int rank){
  int my_start, my_stop;
  int term = m / num_procs;
  //int rest = m % num_procs;
  //if(rest != 0) term++;
  if (rank != 0){
    my_start = term * rank;
  }
  else{
    my_start = 0;
  }
  if (rank != num_procs-1){
    my_stop = term * (rank + 1);
  }
  else{
    my_stop = m;
  }
  int my_m = my_stop - my_start;
  return my_m;

}

void swapper(image *a, image *b)
{// Used to swap position of two arrays.
  image t = *a;
  *a = *b;
  *b = t;
}

void allocate_image(image *u, int m, int n){
  u->m = m;
  u->n = n;
  u->image_data = malloc(m*sizeof(float *));
  u->image_data[0] = malloc(m*n*sizeof(float));
  for (size_t i = 1; i < m; i++) u->image_data[i] = &(u->image_data[i-1][n]);
}

void deallocate_image(image *u){
  free(u->image_data[0]);
  free(u->image_data);
  }

void convert_jpeg_to_image(const unsigned char* image_chars, image *u){
  // Want to fill our 2d array with values from 1d array.
  for (size_t i = 0 ; i<u->m; i ++){
    for (size_t j = 0; j<u->n; j++){
      (u->image_data)[i][j] = image_chars[i*u->n + j];
      // Here we skip the i*n for each iteration in image_chars.
    }
  }
}

void convert_image_to_jpeg(const image *u, unsigned char* image_chars){
  // Want to refill the 1d array with updated values from the 2D array.
  for (size_t i = 0; i<u->m; i++){
    for (size_t j = 0; j<u->n; j++){
      image_chars[i*u->n + j] = (u->image_data)[i][j];
    }
  }
}

void iso_diffusion_denoising_parallel(image *u, image *u_bar,
  float kappa, int iters, int my_rank, int num_procs){
  // Set boundary conditions in horisontal direction:
  double l_odd, l_0, l_l, l_even;
  MPI_Status sta_1, sta_2, s;
  for (int j = 0; j<(u->n); j++){ // i < n means we only go up to n-1, which is
    // what we want. In the parallised version we only want to set these
    // boundary conditions for the first and last process.
    if (my_rank == 0){ // First processer.
      (u_bar->image_data)[0][j] = (u->image_data)[0][j];
    }
    if (my_rank == num_procs-1){ // Last processor.
      (u_bar->image_data)[(u->m)-1][j] = (u->image_data)[(u->m)-1][j];
    }
  }

  // Now we set boundary conditions in the vertical direction. This will be the
  // same for all processes.
  for (int i = 0; i<(u->m); i++){
    (u_bar->image_data)[i][0] = (u->image_data)[i][0];
    (u_bar->image_data)[i][(u->n)-1] = (u->image_data)[i][(u->n)-1];
  }
  // We divide our communication between processors in three classes of sections:
  // the first section, the middle sections and the last section. Before
  // we can execute our algorithm we need to share some information between the
  // processes, namely the ghostpoints.

  for (size_t idx = 0; idx<iters; idx++){
    if (my_rank % 2 != 0) {// Go in here if odd rank.
    // We need to send and receive from the layer above the current one,
    // which also needs to be done by the the last section:

              MPI_Recv(u->image_data[0],
                u->n,
                MPI_FLOAT,
                my_rank-1,
                0,
                MPI_COMM_WORLD,
                &s);
              MPI_Send(u->image_data[1],
                u->n,
                MPI_FLOAT,
                my_rank-1,
                0,
                MPI_COMM_WORLD);

              if (my_rank != num_procs-1){// Go in here if not last process.
              // Then we also Send/Receive information to the layer below
              // this one:

                  MPI_Send(u->image_data[u->m - 2],
                    u->n,
                    MPI_FLOAT,
                    my_rank+1,
                    0,
                    MPI_COMM_WORLD);
                  MPI_Recv(u->image_data[u->m - 1],
                    u->n,
                    MPI_FLOAT,
                    my_rank+1,
                    0,
                    MPI_COMM_WORLD,
                    &s);

              }

          } else { // Go in here if my_rank is even.
            // We also need to send and receive from the layer below
            // the current one, which also needs to be done by the the
            // first section but not for the last:
              if (my_rank != num_procs-1){

                  MPI_Send(u->image_data[u->m - 2],
                    u->n,
                    MPI_FLOAT,
                    my_rank+1,
                    0,
                    MPI_COMM_WORLD);
                  MPI_Recv(u->image_data[u->m - 1],
                    u->n,
                    MPI_FLOAT,
                    my_rank+1,
                    0,
                    MPI_COMM_WORLD,
                    &s);

              }

              // send and receive top
              if (my_rank != 0){ // Go in here if my_rank is not zero. The we
                // also need to Send/Receive information from the layer above
                //this one:

                  MPI_Recv(u->image_data[0],
                    u->n,
                    MPI_FLOAT,
                    my_rank-1,
                    0,
                    MPI_COMM_WORLD,
                    &s);
                  MPI_Send(u->image_data[1],
                    u->n,
                    MPI_FLOAT,
                    my_rank-1,
                    0,
                    MPI_COMM_WORLD);
              }
          }

      for (size_t i = 1; i<(u->m)-1; i++){
        for (size_t j = 1; j<(u->n)-1; j++){

          (u_bar->image_data)[i][j] = (u->image_data)[i][j] + \
          kappa * ( (u->image_data)[i-1][j] + (u->image_data)[i][j-1] - \
          4*(u->image_data)[i][j] + (u->image_data)[i][j+1] + \
          (u->image_data)[i+1][j]);
        }

      }
      swapper(u, u_bar); // Swap pointers.
    }
    swapper(u, u_bar); // Swap back pointers one last time.
  }
