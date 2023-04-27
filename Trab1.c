#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

void transpose_sequential(int **A, int **B, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            B[j][i] = A[i][j];
        }
    }
}

void transpose_parallel(int **A, int **B, int N, int num_threads) {
    #pragma omp parallel for num_threads(num_threads) shared(A, B) schedule(static)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            B[j][i] = A[i][j];
        }
    }
}

int main() {
    int N;

    printf("Digite o tamanho da matriz: ");
    scanf("%d", &N);

    int **A, **B, **C;

    A = (int **) malloc(N * sizeof(int *));
    B = (int **) malloc(N * sizeof(int *));
    C = (int **) malloc(N * sizeof(int *));
    for (int i = 0; i < N; i++) {
        A[i] = (int *) malloc(N * sizeof(int));
        B[i] = (int *) malloc(N * sizeof(int));
        C[i] = (int *) malloc(N * sizeof(int));
    }

    srand(time(NULL));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = rand() % 100;
        }
    }

    double start_time, end_time;
    int threads[] = {2, 4, 8, 16};

    start_time = omp_get_wtime();
    transpose_sequential(A, B, N);
    end_time = omp_get_wtime();
    printf("Tempo sequencial: %f segundos\n", end_time - start_time);

    for (int i = 0; i < 4; i++) {
        start_time = omp_get_wtime();
        transpose_parallel(A, C, N, threads[i]);
        end_time = omp_get_wtime();
        printf("Tempo paralelo com %d threads: %f segundos\n", threads[i], end_time - start_time);
    }

    for (int i = 0; i < N; i++) {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }
    free(A);
    free(B);
    free(C);

    return 0;
}