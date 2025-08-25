#include <stdio.h>
#include <stdlib.h> 
#include <time.h>

// n x n matrix
typedef struct Matrix {
  int n;
  int *data;
} Matrix;

// Allocate memory for n x n matrix as a contiguous block
Matrix* create_matrix(int n) {
  Matrix* m = (Matrix*)malloc(sizeof(Matrix));
  m->n = n;
  m->data = (int*)malloc(n * n * sizeof(int));  
  return m;
}

// Free memory
void free_matrix(Matrix* m) {
  free(m->data);
  free(m);
}

// Generate a random integer between min and max
int generate_random_integer(int min, int max) {
  int random_number = (rand() % (max - min + 1)) + min;
  return random_number;
}

// Fill matrix with random values
void fill_matrix_with_random_values(Matrix* m, int min, int max) {
  int n = m->n;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      m->data[i * n + j] = generate_random_integer(min, max);
    }
  }
}

// Row-major matrix addition
void row_major_matrix_addition(Matrix* A, Matrix* B, Matrix* C) {
  int n = A->n;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      C->data[i * n + j] = A->data[i * n + j] + B->data[i * n + j];
    }
  }
}

// Column-major matrix addition
void column_major_matrix_addition(Matrix* A, Matrix* B, Matrix* C) {
  int n = A->n;
  for (int j = 0; j < n; j++) {
    for (int i = 0; i < n; i++) {
      C->data[i * n + j] = A->data[i * n + j] + B->data[i * n + j];
    }
  }
}

// Average function execution time in milliseconds
typedef void (*matrix_add_func)(Matrix*, Matrix*, Matrix*);

double timespec_diff_ms(struct timespec start, struct timespec end) {
  time_t sec = end.tv_sec - start.tv_sec;
  long nsec = end.tv_nsec - start.tv_nsec;

  if (nsec < 0) {
    sec -= 1;
    nsec += 1000000000L;
  }

  return sec * 1000.0 + nsec / 1e6; 
}

double average_function_execution_time(matrix_add_func f, Matrix* A, Matrix* B, Matrix* C, int n_executions) {
  struct timespec start, end;
  double total_time = 0.0;

  for (int i = 0; i < n_executions; i++) {
    clock_gettime(CLOCK_MONOTONIC, &start);
    f(A, B, C);
    clock_gettime(CLOCK_MONOTONIC, &end);

    total_time += timespec_diff_ms(start, end);
  }

  return total_time / n_executions;
}

typedef struct {
    double value;
    const char *unit;
} ScaledTime;


ScaledTime scale_time(double ms) {
    ScaledTime t;
    if (ms < 1000.0) {
        t.value = ms;
        t.unit = "ms";
    } else if (ms < 60000.0) {
        t.value = ms / 1000.0;
        t.unit = "s";
    } else if (ms < 3600000.0) {
        t.value = ms / 60000.0;
        t.unit = "min";
    } else {
        t.value = ms / 3600000.0;
        t.unit = "h";
    }
    return t;
}

void print_matrix_addition_timings(int* n_values, int num_n_values, int num_executions) {
  int table_column_width = 20;

  printf("\nMatrix addition timings\n");
  printf("--------------------------------------------------------\n");
  printf("%10s | %20s | %20s\n", "n", "Row-major", "Column-major");
  printf("--------------------------------------------------------\n");

  for (int i = 0; i < num_n_values; i++) {
    int n = n_values[i];

    Matrix* A = create_matrix(n);
    Matrix* B = create_matrix(n);
    Matrix* C = create_matrix(n);

    fill_matrix_with_random_values(A, 0, 100);
    fill_matrix_with_random_values(B, 0, 100);

    double row_time = average_function_execution_time(row_major_matrix_addition, A, B, C, num_executions);
    double col_time = average_function_execution_time(column_major_matrix_addition, A, B, C, num_executions);

    ScaledTime row_scaled = scale_time(row_time);
    ScaledTime col_scaled = scale_time(col_time);

    char row_str[table_column_width + 1];
    char col_str[table_column_width + 1];

    snprintf(row_str, sizeof(row_str), "%*.*f %s", 12, 6, row_scaled.value, row_scaled.unit);
    snprintf(col_str, sizeof(col_str), "%*.*f %s", 12, 6, col_scaled.value, col_scaled.unit);

    printf("%10d | %*s | %*s\n", n, table_column_width, row_str, table_column_width, col_str);

    free_matrix(A);
    free_matrix(B);
    free_matrix(C);
  }

  printf("--------------------------------------------------------\n\n");
}

int main()
{
  srand(time(NULL));

  int n_values[] = {128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768};
  int num_n_values = sizeof(n_values) / sizeof(n_values[0]);

  print_matrix_addition_timings(n_values, num_n_values, 5);  

  return 0;
}