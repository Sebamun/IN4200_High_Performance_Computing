# IN4200 Mandatory assignment 2 "Image Denoising"
This is the second mandatory assignment in the course "High-Performance
Computing and Numerical Projects".

## The project used the following coding language and packages:
* C
  * stdlib.h
  * stdio.h
  * math.h
  * mpi.h

## How to run the code for serial:
Open terminal window and go into the serial_code folder. These commands compile and execute the program:
```
# To compile all of the files:
make all
# Run program:
./serial_main mona_lisa_noisy.jpeg mona_lisa_denoised.jpeg 0.2 100
```
Where filename = mona_lisa_noisy.jpeg, output file = mona_lisa_denoised.jpeg,  kappa = 0.2, iters = 100.
You can changes these values as you please.
The compilation arguments is given in the makefile, which you can find in the serial_code folder.

# How to run the code for parallel:
Open terminal window and go into the parallel_code folder. These commands compile and execute the program:
```
# To compile all of the files:
make all
# Run program:
mpirun -n 6 ./parallel_main mona_lisa_noisy.jpeg mona_lisa_denoised.jpeg 0.2 100
```
Where filename = mona_lisa_noisy.jpeg, output file = mona_lisa_denoised.jpeg,  kappa = 0.2, iters = 100.
Here we have that the number after -n is the number of cores you want to use.
You can changes these values as you please.
The compilation arguments is given in the makefile, which you can find in the parallel_code folder.
