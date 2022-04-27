/* needed header files .... */
#include <stdio.h>
#include "common.h" // Import struct from this header file.

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

int main(int argc, char *argv[]) {
      if (argc<3){
        printf("Missing argument(s) in command line");
        exit(0);
      }
       int m, n, c, iters;
       float kappa;
       image u, u_bar; // Define these as image datatype.
       unsigned char *image_chars;
       char *input_jpeg_filename, *output_jpeg_filename;
       /* read from command line: kappa, iters, input_jpeg_filename, output_jpeg_filename */
       input_jpeg_filename = argv[1];
       output_jpeg_filename = argv[2];
       kappa = atof(argv[3]);
       iters = atoi(argv[4]);

       import_JPEG_file(input_jpeg_filename, &image_chars, &m, &n, &c);
       printf("Vertical pixels: %d, Horizontal pixels: %d, num components: %d\n",
       m, n, c); // Printf info about image.
       allocate_image(&u, m, n); // Free up memory for u.
       allocate_image (&u_bar, m, n);
       convert_jpeg_to_image (image_chars, &u);
       iso_diffusion_denoising (&u, &u_bar, kappa, iters);
       convert_image_to_jpeg (&u_bar, image_chars);
       export_JPEG_file(output_jpeg_filename, image_chars, m, n, c, 75);
       deallocate_image (&u);
       deallocate_image (&u_bar);

       return 0;
}
