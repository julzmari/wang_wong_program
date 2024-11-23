#include <stdio.h>
#include <stdlib.h>
#include <string.h>  
#include <float.h>   
#include <time.h>   

extern void saxpy_c(float A, float *X, float *Y, float *Z, int n) {
    for (int i = 0; i < n; i++) {
        Z[i] = A * X[i] + Y[i];
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
    float *Z = (float *)malloc(n * sizeof(float));

    if (!X || !Y || !Z) {
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

    //timing the SAXPY kernel
    clock_t start = clock();  // start time
    printf("Start time (clock ticks): %ld\n", start);  // Print start time

    // SAXPY function to compute the result in vector Z
    saxpy_c(A, X, Y, Z, n);

   
    clock_t end = clock();  // Record the end time
    printf("End time (clock ticks): %ld\n", end);  // Print end time

    //time taken
    double time_taken = (double)(end - start) / CLOCKS_PER_SEC;
    printf("SAXPY kernel executed in %.6f seconds\n", time_taken);

    printf("First 10 elements of vector Z:\n");
    for (int i = 0; i < (n < 10 ? n : 10); i++) {
        printf("%.1f ", Z[i]);
    }
    printf("\n");

    free(X);
    free(Y);
    free(Z);

    return 0;
}

