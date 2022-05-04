typedef struct{ // We want to rename this struct as a new type.

  float** image_data;  // a 2D array of floats.
  int m;               // # pixels in vertical-direction.
  int n;               // # pixels in horizontal-direction.

}image; // This is the name of the new struct.
// Need to state the functions we will be using in header file:
void allocate_image(image *u, int m, int n);
void convert_jpeg_to_image(const unsigned char* image_chars, image *u);
void convert_image_to_jpeg(const image *u, unsigned char* image_chars);
void iso_diffusion_denoising_parallel(image *u, image *u_bar, float kappa, int iters,int my_rank, int num_procs);
void deallocate_image(image *u);
