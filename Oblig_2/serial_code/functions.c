#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "common.h" // Import struct from this header file.

void swapper(image *a, image *b)
{// Used to swap position of two arrays.
  image t = *a;
  *a = *b;
  *b = t;
}

void allocate_image(image *u, int m, int n){
  // Here we should allocate memory for u.
  u->m = m;
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

void iso_diffusion_denoising(image *u, image *u_bar, float kappa, int iters){
  // Set boundary conditions in horisontal direction:
  for (int j = 0; j<(u->n); j++){ // i < n means we only go up to n-1, which is
    // what we want.
    (u_bar->image_data)[(u->m)-1][j] = (u->image_data)[(u->m)-1][j];
    (u_bar->image_data)[0][j] = (u->image_data)[0][j];
  }
  // Now we set boundary conditions in the vertical direction.
  for (int i = 0; i<(u->m); i++){
    (u_bar->image_data)[i][0] = (u->image_data)[i][0];
    (u_bar->image_data)[i][(u->n)-1] = (u->image_data)[i][(u->n)-1];

  }
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
}

void deallocate_image(image *u){
  for (size_t i = 0; i<u->m; i++) free((u->image_data)[i]);
  free(u->image_data);
}
