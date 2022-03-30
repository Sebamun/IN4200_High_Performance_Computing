#include <stdio.h>

void swapper(int* xp, int* yp)
{
    int t = *xp;
    *xp = *yp;
    *yp = t;
}

// Function to perform Selection Sort
void Sort(int arr[], int arr_2[], int n)
{
    int i, j, min_idx; // Iterative indexes + min index.

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







// Tried to use these functions first but there was some memory issue:
void swap(int *a, int *b) {
    int t=*a; *a=*b; *b=t;
}

void sort_perm(int *arr, int beg, int end) {
    if (end > beg + 1) { // Continue if beg + 1 is still less than end.

        int piv = arr[beg]; // Set pivot to beg element.
        int l = beg + 1; // Next element from pivot element.
        int r = end; // Last element we look at.
        while (l < r) {
            if (arr[l] <= piv) // Compare value with pivot element.
              l++; // Add +1 to l.
            else
                swap(&arr[l], &arr[--r]); // Swap element l with r-1.
        }
        swap(&arr[--l], &arr[beg]);
        sort_perm(arr, beg, l);
        sort_perm(arr, r, end); // So each time the condition in the if test is
        //met we call on the function recursively.
    }
}

/*

void swap(int *a, int *b) {
    int t=*a; *a=*b; *b=t;
}

void sort_perm(int *arr, int beg, int end) {
    if (end > beg + 1) { // Continue if beg + 1 is still less than end.

        int piv = arr[beg] // Set pivot to beg element.
        int l = beg + 1 // Next element from pivot element.
        int r = end; // Last element we look at.
        while (l < r) {
            if (arr[l] <= piv) // Compare value with pivot element.
              l++; // Add +1 to l.
            else
                swap(&arr[l], &arr[--r]);
        }
        swap(&arr[--l], &arr[beg]);
        sort_perm(arr, beg, l);
        sort_perm(arr, r, end);
    }
}
*/
