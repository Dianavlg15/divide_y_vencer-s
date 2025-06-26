#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int **allocate_matrix(int n) {
    int **mat = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++)
        mat[i] = (int *)calloc(n, sizeof(int));
    return mat;
}

void free_matrix(int **mat, int n) {
    for (int i = 0; i < n; i++)
        free(mat[i]);
    free(mat);
}

void fill_random(int **mat, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            mat[i][j] = rand() % 10;
}

int **add(int **A, int **B, int n) {
    int **C = allocate_matrix(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = A[i][j] + B[i][j];
    return C;
}

int **sub(int **A, int **B, int n) {
    int **C = allocate_matrix(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = A[i][j] - B[i][j];
    return C;
}

int **traditional(int **A, int **B, int n) {
    int **C = allocate_matrix(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            for (int k = 0; k < n; k++)
                C[i][j] += A[i][k] * B[k][j];
    return C;
}

int **dac(int **A, int **B, int n) {
    if (n == 1) {
        int **C = allocate_matrix(1);
        C[0][0] = A[0][0] * B[0][0];
        return C;
    }

    int mid = n / 2;
    int **A11 = allocate_matrix(mid), **A12 = allocate_matrix(mid),
         **A21 = allocate_matrix(mid), **A22 = allocate_matrix(mid),
         **B11 = allocate_matrix(mid), **B12 = allocate_matrix(mid),
         **B21 = allocate_matrix(mid), **B22 = allocate_matrix(mid);

    for (int i = 0; i < mid; i++)
        for (int j = 0; j < mid; j++) {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j + mid];
            A21[i][j] = A[i + mid][j];
            A22[i][j] = A[i + mid][j + mid];
            B11[i][j] = B[i][j];
            B12[i][j] = B[i][j + mid];
            B21[i][j] = B[i + mid][j];
            B22[i][j] = B[i + mid][j + mid];
        }

    int **C11 = add(dac(A11, B11, mid), dac(A12, B21, mid), mid);
    int **C12 = add(dac(A11, B12, mid), dac(A12, B22, mid), mid);
    int **C21 = add(dac(A21, B11, mid), dac(A22, B21, mid), mid);
    int **C22 = add(dac(A21, B12, mid), dac(A22, B22, mid), mid);

    int **C = allocate_matrix(n);
    for (int i = 0; i < mid; i++)
        for (int j = 0; j < mid; j++) {
            C[i][j] = C11[i][j];
            C[i][j + mid] = C12[i][j];
            C[i + mid][j] = C21[i][j];
            C[i + mid][j + mid] = C22[i][j];
        }

    free_matrix(A11, mid); free_matrix(A12, mid); free_matrix(A21, mid); free_matrix(A22, mid);
    free_matrix(B11, mid); free_matrix(B12, mid); free_matrix(B21, mid); free_matrix(B22, mid);
    free_matrix(C11, mid); free_matrix(C12, mid); free_matrix(C21, mid); free_matrix(C22, mid);

    return C;
}

int **strassen(int **A, int **B, int n) {
    if (n == 1) {
        int **C = allocate_matrix(1);
        C[0][0] = A[0][0] * B[0][0];
        return C;
    }

    int mid = n / 2;
    int **A11 = allocate_matrix(mid), **A12 = allocate_matrix(mid),
         **A21 = allocate_matrix(mid), **A22 = allocate_matrix(mid),
         **B11 = allocate_matrix(mid), **B12 = allocate_matrix(mid),
         **B21 = allocate_matrix(mid), **B22 = allocate_matrix(mid);

    for (int i = 0; i < mid; i++)
        for (int j = 0; j < mid; j++) {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j + mid];
            A21[i][j] = A[i + mid][j];
            A22[i][j] = A[i + mid][j + mid];
            B11[i][j] = B[i][j];
            B12[i][j] = B[i][j + mid];
            B21[i][j] = B[i + mid][j];
            B22[i][j] = B[i + mid][j + mid];
        }

    int **M1 = strassen(add(A11, A22, mid), add(B11, B22, mid), mid);
    int **M2 = strassen(add(A21, A22, mid), B11, mid);
    int **M3 = strassen(A11, sub(B12, B22, mid), mid);
    int **M4 = strassen(A22, sub(B21, B11, mid), mid);
    int **M5 = strassen(add(A11, A12, mid), B22, mid);
    int **M6 = strassen(sub(A21, A11, mid), add(B11, B12, mid), mid);
    int **M7 = strassen(sub(A12, A22, mid), add(B21, B22, mid), mid);

    int **C11 = add(sub(add(M1, M4, mid), M5, mid), M7, mid);
    int **C12 = add(M3, M5, mid);
    int **C21 = add(M2, M4, mid);
    int **C22 = add(sub(add(M1, M3, mid), M2, mid), M6, mid);

    int **C = allocate_matrix(n);
    for (int i = 0; i < mid; i++)
        for (int j = 0; j < mid; j++) {
            C[i][j] = C11[i][j];
            C[i][j + mid] = C12[i][j];
            C[i + mid][j] = C21[i][j];
            C[i + mid][j + mid] = C22[i][j];
        }

    free_matrix(A11, mid); free_matrix(A12, mid); free_matrix(A21, mid); free_matrix(A22, mid);
    free_matrix(B11, mid); free_matrix(B12, mid); free_matrix(B21, mid); free_matrix(B22, mid);
    free_matrix(M1, mid); free_matrix(M2, mid); free_matrix(M3, mid); free_matrix(M4, mid);
    free_matrix(M5, mid); free_matrix(M6, mid); free_matrix(M7, mid);
    free_matrix(C11, mid); free_matrix(C12, mid); free_matrix(C21, mid); free_matrix(C22, mid);

    return C;
}

int main() {
    int n;
    printf("Ingresa el tamaño n de las matrices (potencia de 2): ");
    scanf("%d", &n);

    srand(time(NULL));
    int **A = allocate_matrix(n);
    int **B = allocate_matrix(n);
    fill_random(A, n);
    fill_random(B, n);

    clock_t start, end;
    double time_used;

    start = clock();
    int **C1 = traditional(A, B, n);
    end = clock();
    time_used = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;
    printf("Tiempo Método Tradicional: %.2f ms\n", time_used);
    free_matrix(C1, n);

    start = clock();
    int **C2 = dac(A, B, n);
    end = clock();
    time_used = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;
    printf("Tiempo Divide & Conquer: %.2f ms\n", time_used);
    free_matrix(C2, n);

    start = clock();
    int **C3 = strassen(A, B, n);
    end = clock();
    time_used = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;
    printf("Tiempo Strassen: %.2f ms\n", time_used);
    free_matrix(C3, n);

    free_matrix(A, n);
    free_matrix(B, n);

    return 0;
}
