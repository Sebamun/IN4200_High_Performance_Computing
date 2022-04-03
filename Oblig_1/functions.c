#include <stdio.h>

void swapper(int* xp, int* yp)
{// Used to swap position of two integer elements in array.
    int t = *xp;
    *xp = *yp;
    *yp = t;
}

void swapper_2(double* xp, double* yp)
{ // Used to swap doubles.
    double t = *xp;
    *xp = *yp;
    *yp = t;
}

void Sort(int arr[], int arr_2[], int n)
{ // This function sorts two arrays, one in descending order, the other follows
  // the same changes as in the first array.
    int i;
    int j;
    int min_idx;

    // One by one move boundary of unsorted subarray
    for (i = 0; i < n; i++) { // Run through the whole array.
        // Find the minimum element in unsorted array
        min_idx = i;
        for (j = i + 1; j < n; j++)
            if (arr[j] < arr[min_idx])
                min_idx = j;

        // Swap the found mimimum element with the first element.
        swapper(&arr[min_idx], &arr[i]);
        swapper(&arr_2[min_idx], &arr_2[i]);
    }
}

void Sort_2(double arr[], int arr_2[], int n)
{
    int i;
    int j;
    int max_idx;

    // One by one move boundary of unsorted subarray
    for (i = 0; i < n; i++) { // Run through the whole array.
        // Find the maximum element in unsorted array
        max_idx = i;
        for (j = i + 1; j < n; j++)
            if (arr[j] > arr[max_idx])
                max_idx = j; // New max index.

        // Swap the found maximum element with the first element.
        swapper_2(&arr[max_idx], &arr[i]);
        swapper(&arr_2[max_idx], &arr_2[i]);
    }
}

double largest(double arr[], int n)
{
    int i;
    double max = arr[0]; // Initialize maximum element.
    // Go through every element and compare with the current max.
    for (i = 1; i < n; i++)
        if (arr[i] > max)
            max = arr[i];

    return max;
}

// Tried to use some other sort functions (quicksort) first but there was some
// memory issues. If we implemented these functions the read file would most
// likely sort the data much quicker then it currently does,
// which would help us read the larger files.
