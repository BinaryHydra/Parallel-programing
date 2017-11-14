#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#ifdef debug
#define D(a) a
#else
#define D(a)
#endif

void genMatrix(int *A, int N, int M) {
    // Clean matrix
    for (int i = 0; i < N * M; i++) {
        A[i] = 0;
    }
    // Generate matrix
    int m = M / 4;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < m; j++) A[i * M + j] = (int)((double)rand() / RAND_MAX * 99) + 1;
        if (i > 0 && (i + 1) % (N / 4) == 0) m += M / 4;
    }
}

int main(int argc, char* argv[]) {
    int rank, procCount; 
    int N = 16;_
    int M = 20;
    int *A = new int[N * M];
    int *workMatrix = new int[N*M/procCount];
    int t, n;
    double median[N];
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &procCount);
    if (rank == 0) {
        srand(time(NULL));
        genMatrix(A, N, M);
    }
    MPI_Scatter(A, N*M/4, MPI_INT, workMatrix, N*M/4/procCount, MPI_INT, 0, MPI_COMM_WORLD);
    
/*    for (int rowNo = 0; rowNo < N; rowNo++) {
        n = 0;
        while (A[rowNo * M + n] != 0 && n < M) {
            n++;
        }
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - 1; j++) {
                if (A[rowNo * M + j] > A[rowNo * M + j + 1]) {
                    t = A[rowNo * M + j];
                    A[rowNo * M + j] = A[rowNo * M + j + 1];
                    A[rowNo * M + j + 1] = t;
                }
            }
        }
        if (n % 2 == 1) {
            median[rowNo] = (double)A[rowNo * M + (n / 2)];
        } else {
            median[rowNo] = (double(A[rowNo * M + (n / 2)] + A[rowNo * M + (n / 2) - 1]) / 2);
        }
    }
*/
// Print matrix
for (int i = 0; i < N/4/procCount; i++) {
    //printf("median = %3.3f", median[i]);
    for (int j = 0; j < M; j++) {
        printf("%3d", workMatrix[i * M + j]);
    }
    printf("\n");
}
}