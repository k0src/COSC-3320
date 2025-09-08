#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

// A global variable for the timer frequency to avoid re-querying it
static LARGE_INTEGER frequency;

// A simple function to allocate memory and return a pointer to the array of arrays
int** allocate_memory(int n, int bytes) {
    // Create the array to hold the pointers
    int** arrays = (int**)malloc(n * sizeof(int*));
    if (!arrays) {
        return NULL;
    }

    // Allocate memory for each individual array
    for (int i = 0; i < n; i++) {
        arrays[i] = (int*)malloc(bytes);
        if (!arrays[i]) {
            // Clean up already allocated memory on failure
            for (int j = 0; j < i; j++) {
                free(arrays[j]);
            }
            free(arrays);
            return NULL;
        }
    }
    return arrays;
}

// A simple function to deallocate memory for even-numbered arrays
void deallocate_even_arrays(int** arrays, int n) {
    for (int i = 0; i < n; i += 2) {
        if (arrays[i]) {
            free(arrays[i]);
            arrays[i] = NULL;
        }
    }
}

int main() {
    LARGE_INTEGER start, end;
    double elapsed_time;
    const int M = 100;
    
    // Get the timer frequency once at the beginning
    QueryPerformanceFrequency(&frequency);

    // --- Task 1: Time for allocation of 3m arrays of 1 MB ---
    printf("Timing allocation of %d 1 MB arrays...\n", 3 * M);
    int n1 = 3 * M;
    int bytes1 = 1000000;
    int** arrays1;

    QueryPerformanceCounter(&start);
    arrays1 = allocate_memory(n1, bytes1);
    QueryPerformanceCounter(&end);

    if (arrays1) {
        elapsed_time = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;
        printf("Allocation time: %f seconds\n\n", elapsed_time);
    } else {
        printf("Memory allocation failed for task 1.\n");
        return 1;
    }

    // --- Task 2: Time for deallocation of even arrays ---
    printf("Timing deallocation of even-numbered arrays...\n");

    QueryPerformanceCounter(&start);
    deallocate_even_arrays(arrays1, n1);
    QueryPerformanceCounter(&end);
    
    elapsed_time = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;
    printf("Deallocation time: %f seconds\n\n", elapsed_time);

    // Free the remaining odd-numbered arrays and the main pointer array
    for (int i = 1; i < n1; i += 2) {
        free(arrays1[i]);
    }
    free(arrays1);

    // --- Task 3: Time for allocation of m arrays of 1.45 MB ---
    printf("Timing allocation of %d 1.45 MB arrays...\n", M);
    int n2 = M;
    int bytes2 = 1450000;
    int** arrays2;

    QueryPerformanceCounter(&start);
    arrays2 = allocate_memory(n2, bytes2);
    QueryPerformanceCounter(&end);
    
    if (arrays2) {
        elapsed_time = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;
        printf("Allocation time: %f seconds\n\n", elapsed_time);
    } else {
        printf("Memory allocation failed for task 3.\n");
        return 1;
    }

    // Free all allocated memory for the second task
    for (int i = 0; i < n2; i++) {
        free(arrays2[i]);
    }
    free(arrays2);

    return 0;
}
