#include <stdio.h>
#include <stdlib.h>
#include <string.h>  
#include <float.h>   
#include <time.h> 
#include <math.h>  

extern void saxpy_asm(int n, float* X, float* Y, float* Z, float A);

void saxpy_c(float A, float *X, float *Y, float *Z_c, int n) {
    for (int i = 0; i < n; i++) {
        Z_c[i] = A * X[i] + Y[i]; 
        //printf("Y[%d] = %f\n", i, Y[i]);
    }
}

int read_float(float *value) {
    char buffer[100];
    char *endptr;

    if (fgets(buffer, sizeof(buffer), stdin)) {
        buffer[strcspn(buffer, "\n")] = 0;

        if (strlen(buffer) == 0) {
            return 0;
        }

        if (strchr(buffer, '.') == NULL) {
            return 0;
        }

        *value = strtof(buffer, &endptr);

        if ((*endptr == '\0' || *endptr == '\n') && *value >= FLT_MIN && *value <= FLT_MAX) {
            return 1;
        }
    }
    return 0;
}

int read_int(int *value) {
    char buffer[100];
    char *endptr;

    if (fgets(buffer, sizeof(buffer), stdin)) {
        buffer[strcspn(buffer, "\n")] = 0;

        if (strlen(buffer) == 0) {
            return 0;
        }

        *value = strtol(buffer, &endptr, 10);

        return (*endptr == '\0' || *endptr == '\n');
    }
    return 0;
}

int compare_float(float a, float b, float tolerance) {
    return fabs(a - b) <= tolerance;
}

void check_correctness(float *Z_c, float *Z, int n) {
    float tolerance = 1e-6;  // Acceptable tolerance for floating-point comparison
    int correct = 1;         // Assume correctness

    for (int i = 0; i < n; i++) {
        if (!compare_float(Z_c[i], Z[i], tolerance)) {
            printf("Mismatch at index %d: C = %.6f, ASM = %.6f\n", i, Z_c[i], Z[i]);
            correct = 0;
        }
    }

    if (correct) {
        printf("Results match within tolerance.\n");
    } else {
        printf("Results mismatch found.\n");
    }
}


int main() {
    int n;
    float A;

    printf("Enter scalar A (single-precision float): ");
    while (!read_float(&A)) {
        printf("Invalid input for A. \nPlease enter a valid single-precision float: ");
    }

    printf("Enter vector size (n): ");
    while (!read_int(&n) || n <= 0) {
        printf("Invalid input for vector size. \nPlease enter a positive integer: ");
    }

    float *X = (float *)malloc(n * sizeof(float));
    float *Y = (float *)malloc(n * sizeof(float));
    float *Z_c = (float *)malloc(n * sizeof(float));  // C version result
    float *Z = (float *)malloc(n * sizeof(float)); // ASM version result

    if (!X || !Y || !Z_c || !Z) {
        printf("Memory allocation failed!\n");
        return 0;
    }

    printf("Enter values for vector X (size %d):\n", n);
    for (int i = 0; i < n; i++) {
        printf("X[%d] = ", i);
        while (!read_float(&X[i])) {
            printf("Invalid input for X[%d]. \nPlease enter a valid single-precision float: ", i);
        }
    }

    printf("Enter values for vector Y (size %d):\n", n);
    for (int i = 0; i < n; i++) {
        printf("Y[%d] = ", i);
        while (!read_float(&Y[i])) {
            printf("Invalid input for Y[%d]. \nPlease enter a valid single-precision float: ", i);
        }
    }

   
	// Timing the assembly SAXPY kernel
    clock_t start_asm = clock();  // start time for ASM version
    printf("Start time x86-64 (clock ticks): %ld\n", start_asm);

    // (ASM version)
    saxpy_asm(n, X, Y, Z,A);

    clock_t end_asm = clock();  // end time for ASM version
    printf("End time x86-64 (clock ticks): %ld\n", end_asm);

    double time_taken_asm = (double)(end_asm - start_asm) / CLOCKS_PER_SEC;
    printf("x86-64  SAXPY kernel executed in %.6f seconds\n", time_taken_asm);

    printf("\nFirst 10 elements of vector Z (ASM version):\n");
    for (int i = 0; i < (n < 10 ? n : 10); i++) {
        printf("%.1f ", Z[i]);
    }
    printf("\n");
    
  
    // Timing the C SAXPY kernel
    clock_t start_c = clock();  // start time for C version
    printf("\nStart time C kernel (clock ticks): %ld\n", start_c);
    
    // (C version)
    saxpy_c(A, X, Y, Z_c, n);

    clock_t end_c = clock();  // end time for C version
    printf("End time C kernel (clock ticks): %ld\n", end_c);

    double time_taken_c = (double)(end_c - start_c) / CLOCKS_PER_SEC;
    printf("C SAXPY kernel executed in %.6f seconds\n", time_taken_c);

    printf("\nFirst 10 elements of vector Z (C version):\n");
    for (int i = 0; i < (n < 10 ? n : 10); i++) {
        printf("%.1f ", Z_c[i]);
    }
    printf("\n");
    printf("\n");
    
    // Check the correctness of both results
    check_correctness(Z_c, Z, n);
    
    free(X);
    free(Y);
    free(Z_c);
    free(Z);

    return 0;
}


