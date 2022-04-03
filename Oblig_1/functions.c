#include <stdio.h>

void swapper(int* xp, int* yp)
{// Used to swap position of two elements in array.
    int t = *xp;
    *xp = *yp;
    *yp = t;
}

void swapper_2(double* xp, double* yp)
{
    double t = *xp;
    *xp = *yp;
    *yp = t;
}



void Sort(int arr[], int arr_2[], int n)
{ // This function sorts two array, one in descending order, the other follows
  // the same changes as in the first array.
    int i, j, min_idx;

    // One by one move boundary of unsorted subarray
    for (i = 0; i < n; i++) { // Run through the whole array.
        // Find the minimum element in unsorted array
        min_idx = i;
        for (j = i + 1; j < n; j++)
            if (arr[j] < arr[min_idx])
                min_idx = j;

        // Swap the found minimum element
        // with the first element
        swapper(&arr[min_idx], &arr[i]);
        swapper(&arr_2[min_idx], &arr_2[i]);
    }
}

void Sort_2(double arr[], int arr_2[], int n)
{
    int i, j, min_idx; // Iterative indexes + min index.

    // One by one move boundary of unsorted subarray
    for (i = 0; i < n; i++) { // Run through the whole array.
        // Find the minimum element in unsorted array
        min_idx = i;
        for (j = i + 1; j < n; j++)
            if (arr[j] > arr[min_idx])
                min_idx = j;

        // Swap the found minimum element
        // with the first element
        swapper_2(&arr[min_idx], &arr[i]);
        swapper(&arr_2[min_idx], &arr_2[i]);
    }
}

double largest(double arr[], int n)
{
    int i;

    // Initialize maximum element
    double max = arr[0];

    // Traverse array elements
    // from second and compare
    // every element with current max
    for (i = 1; i < n; i++)
        if (arr[i] > max)
            max = arr[i];

    return max;
}

// Tried to use these functions first but there was some memory issues. If
// we implemented these functions the read file would most likely
// sort the data much quicker then it currently does, which would help us read
// the larger files.
