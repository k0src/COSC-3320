#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

const int ARRAY_SIZE_1_MB = 1024 * 1024;
const int ARRAY_SIZE_145_MB = (int)(1.45 * 1024 * 1024);

double getTime() {
 struct timespec ts;
 clock_gettime(CLOCK_MONOTONIC, &ts);
 return (double) ts.tv_sec + (double) ts.tv_nsec / 1e9;
}

void freeArrays(void** arrays, int count) {
 for (int i = 0; i < count; i++) {
   if (arrays[i] != NULL) {
     free(arrays[i]);
     arrays[i] = NULL;
   }
 }
}

int main(int argc, char *argv[])
{
 int M;
  if (argc != 2) {
   fprintf(stderr, "Usage: %s M\n", argv[0]);
   return 1;
 }

 if (sscanf(argv[1], "%d", &M) != 1 || M <= 0) {
   fprintf(stderr, "Invalid M value\n");
   return 1;
 }

 printf("\nMemory Fragmentation in C\n");
 printf("M = %d\n", M);
 printf("Array size 1: %d bytes (%.2f MB)\n", ARRAY_SIZE_1_MB, (float) ARRAY_SIZE_1_MB / (1024 * 1024));
 printf("Array size 2: %d bytes (%.2f MB)\n\n", ARRAY_SIZE_145_MB, (float) ARRAY_SIZE_145_MB / (1024 * 1024));

  // 1. Allocate 3M arrays of size 1 MB each
 printf("Allocating %d arrays of size 1 MB each...\n", 3 * M);

 // Array of pointers to allocated arrays (1 MB)
 void** arrays = (void**) malloc(3 * M * sizeof(void*));
 if (arrays == NULL) {
   fprintf(stderr, "Memory allocation failed\n");
   return 1;
 }

 // Initalize all pointers to NULL
 for (int i = 0; i < 3 * M; i++) {
   arrays[i] = NULL;
 }

 double startTime = getTime();

 for (int i = 0; i < 3 * M; i++) {
   arrays[i] = malloc(ARRAY_SIZE_1_MB);
   if (arrays[i] == NULL) {
     fprintf(stderr, "Memory allocation failed at iteration %d\n", i);
     // Free allocated memory
     freeArrays(arrays, i);
     return 1;
   }
   // Initialize the array to avoid lazy allocation
   memset(arrays[i], 0, ARRAY_SIZE_1_MB);
 }

 double endTime = getTime();
 double allocationTime1 = endTime - startTime;

 printf("Allocation time for %d 1 MB arrays: %.6f seconds\n\n", 3 * M, allocationTime1);

 // 2. Free even-numbered arrays
 printf("De-allocating memory for every even-numbered array (~%d arrays)...\n", (3 * M) / 2);

 startTime = getTime();

 for (int i = 0; i < 3 * M; i += 2) {
   free(arrays[i]);
   arrays[i] = NULL;
 }

 endTime = getTime();
 double deallocationTime = endTime - startTime;

 printf("De-allocation time ~%d even-numbered arrays: %.6f seconds\n\n", (3 * M) / 2, deallocationTime);

 // 3. Allocate M arrays of size 1.45 MB each
 printf("Allocating %d arrays of size 1.45 MB each...\n", M);

 // Array of pointers to allocated arrays (1.45 MB)
 void** arrays2 = (void**) malloc(M * sizeof(void*));
 if (arrays2 == NULL) {
   fprintf(stderr, "Memory allocation failed\n");
   // Free old memory
   freeArrays(arrays, 3 * M);
   return 1;
 }
 // Initialize all pointers to NULL
 for (int i = 0; i < M; i++) {
   arrays2[i] = NULL;
 }

 startTime = getTime();

 for (int i = 0; i < M; i++) {
   arrays2[i] = malloc(ARRAY_SIZE_145_MB);
   if (arrays2[i] == NULL) {
     fprintf(stderr, "Memory allocation failed at iteration %d\n", i);
     // Free allocated memory
     freeArrays(arrays2, i);
     // Free old memory
     freeArrays(arrays, 3 * M);
     return 1;
   }
   // Initialize the array to avoid lazy allocation
   memset(arrays2[i], 0, ARRAY_SIZE_145_MB);
 }

 endTime = getTime();
 double allocationTime2 = endTime - startTime;

 printf("Allocation time for %d 1.45 MB arrays: %.6f seconds\n\n", M, allocationTime2);
 printf("Ratio (allocation time for 1.45 MB arrays / allocation time for 1 MB arrays): %.2f\n\n", allocationTime2 / allocationTime1);

 // Free all allocated memory
 freeArrays(arrays, 3 * M);
 freeArrays(arrays2, M);
 free(arrays);
 free(arrays2);

 return 0;
}

