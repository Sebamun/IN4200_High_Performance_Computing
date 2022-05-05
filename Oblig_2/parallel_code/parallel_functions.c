#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#include "parallel_common.h" // Import struct from this header file.

void swapper(image *a, image *b)
{// Used to swap position of two arrays.
  image t = *a;
  *a = *b;
  *b = t;
}

void allocate_image(image *u, int m, int n){
  // Here we should allocate memory for u.
  u->m = m; // In practise these will be the my_m and my_n values.
  u->n = n;
  u->image_data = malloc(m * sizeof(float*));
  for (size_t i = 0; i<m; i++){
    (u->image_data)[i] = malloc(n * sizeof(float));
  }
}

void convert_jpeg_to_image(const unsigned char* image_chars, image *u){
  // Want to fill our 2d array with values from 1d array.
  // for (int i = 0; i<(u->m * u->n); i++ )printf("%d \n", image_chars[i]);
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

void iso_diffusion_denoising_parallel(image *u, image *u_bar, float kappa, int iters, int my_rank, int num_procs){
  // Set boundary conditions in horisontal direction:
  double l_odd, l_0, l_l, l_even;
  MPI_Status sta_1, sta_2;
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
  // We divide our communication between processors in three sections. Before
  // we can execute our algorithm we need to share some information between the
  // processes, namely the ghostpoints. First we allocate these rows as pointers:

  float *toplayer = malloc(u->n * sizeof(float));
  float *botlayer = malloc(u->n * sizeof(float));

  for (size_t idx = 0; idx<iters; idx++){


  if (my_rank%2){ // Go in here if odd rank.
    if (my_rank != (num_procs-1)){ // Go in here if not last process. Then we also
      // Send/Receive information to the layer below this one:
      botlayer = (u->image_data)[(u->m-1)-1]; // Second to last row.
      MPI_Send(botlayer, u->n, MPI_DOUBLE, my_rank+1, 0, MPI_COMM_WORLD);
      // Receive information from the layer below this one:
      MPI_Recv(botlayer, u->n, MPI_DOUBLE, my_rank+1, 0, MPI_COMM_WORLD, &sta_1);
      // We can now add a extra ghost row to our section.
      u->image_data[u->m - 1] = botlayer;
    }
    printf("hei\n");


    // We also need to send and receive from the layer above the current one,
    // which also needs to be done by the the last section:
    toplayer = (u->image_data)[1]; // Second row.
    // Send information to the layer above this one:
    MPI_Send(toplayer, u->n, MPI_DOUBLE, my_rank-1, 0, MPI_COMM_WORLD);
    // We also need to receive information from the layer above the current one:
    MPI_Recv(toplayer, u->n, MPI_DOUBLE, my_rank-1, 0, MPI_COMM_WORLD, &sta_2);
    (u->image_data)[0] = toplayer; // This becomes our first row (of ghostpoints).
  }
  else{ // This is if my_rank is even:
    if (my_rank != 0){ // Go in here if my_rank is different from zero. The we
      // also need to Send/Receive information from the layer above this one:
      toplayer = (u->image_data)[1]; // Second row.
      MPI_Send(toplayer, u->n, MPI_DOUBLE, my_rank-1, 0, MPI_COMM_WORLD);
      MPI_Recv(toplayer, u->n, MPI_DOUBLE, my_rank-1, 0, MPI_COMM_WORLD, &sta_2);
      (u->image_data)[0] = toplayer; // This becomes our first row (of ghostpoints).
    }
    // We also need to send and receive from the layer below the current one,
    // which also needs to be done by the the first section:
    botlayer = (u->image_data)[(u->m-1)-1]; // Second to last row.
    MPI_Send(botlayer, u->n, MPI_DOUBLE, my_rank+1, 0, MPI_COMM_WORLD);
    // Receive information from the layer below this one:
    MPI_Recv(botlayer, u->n, MPI_DOUBLE, my_rank+1, 0, MPI_COMM_WORLD, &sta_1);
    // We can now add a extra ghost row to our section.
    u->image_data[u->m - 1] = botlayer;
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




// Comment: The processor will hang when it sends a chunk of data, untill you
// receive it in another processor. 






// Now we go to the last section, which wants to receive from values
// from the first row in the previous section and send its first row to
// the previous section.
/*
if (my_rank == (num_procs - 1)){
  // Then we want to receive the last row from the previous section:
  MPI_Irecv(&(u->image_data)[0][i], 1, MPI_DOUBLE, my_rank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &req_1);
  //(u->image_data)[0][i] = l_even;
}
if (my_rank == 0){
  // We also want to receive the upcoming elements in the first row of the
  // next section:
  MPI_Irecv(&(u->image_data)[(u->m)][i], 1, MPI_DOUBLE, my_rank + 1, MPI_ANY_TAG, MPI_COMM_WORLD, &req_2);
  //(u->image_data)[u->m][i] = l_odd;
}
if (my_rank == (num_procs - 1)){
  // First we send the first row back to the previous section:
  //l_l = (u->image_data)[1][i];
  MPI_Send(&(u->image_data)[1][i], 1, MPI_DOUBLE, my_rank-1, 0, MPI_COMM_WORLD);
}
// We begin with the upper section, which do not receive data from a section
// over it. This is the case for my_rank = 0:
if (my_rank == 0){
  // We send the last row values in the to the next section:
  //l_0 = (u->image_data)[u->m-1][i];
  MPI_Send(&(u->image_data)[(u->m)-1][i], 1, MPI_DOUBLE, my_rank+1, 0, MPI_COMM_WORLD);
}

//MPI_Barrier(MPI_COMM_WORLD);

if (my_rank == (num_procs - 1)){
  MPI_Wait(&req_1, MPI_STATUS_IGNORE);
}
if (my_rank == 0){
  MPI_Wait(&req_2, MPI_STATUS_IGNORE);
}
*/













/*        if (my_rank%2 && my_rank != 0){
          printf("%d", my_rank);
          //printf("%d \n", iters);
          //u_up_1 = (u->image_data)[0][i];
          //MPI_Send(&u_up, 1, MPI_DOUBLE, my_rank, 0, MPI_COMM_WORLD);
        }


        //if {
          //printf("%d", my_rank);
          //MPI_Recv(&u_up, 1, MPI_DOUBLE, my_rank + 1, MPI_ANY_TAG, MPI_COMM_WORLD, &s);
          //(u->image_data)[u->m+1][i] = u_up;
        //}

*/



/*
if (my_rank>0){
  u_up = (u->image_data)[0][i];
  MPI_Send(&u_up, 1, MPI_DOUBLE, my_rank, 0, MPI_COMM_WORLD);
}
if (my_rank == 0){
  MPI_Recv(&u_under, 1, MPI_DOUBLE, my_rank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &s);
  (u->image_data)[u->m+1][i] = u_under;
}
*/








        //(u_bar->image_data)[i][j] = (u->image_data)[i][j] + \
        //kappa * ( (u->image_data)[i-1][j] + (u->image_data)[i][j-1] - \
        //4*(u->image_data)[i][j] + (u->image_data)[i][j+1] + \
        //(u->image_data)[i+1][j]);


  /*
  // Fill in the remainig values for u_bar:
  for (size_t idx = 0; idx<iters; idx++){
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
  swapper(u, u_bar); // Swap back pointers on last time.
  */

void deallocate_image(image *u){
  for (size_t i = 0; i<u->m; i++) free((u->image_data)[i]);
  free(u->image_data);
}
