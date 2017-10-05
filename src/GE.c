#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
  int i;
  int jStart;
  int k;
  int N;
  int count;
  int threadIndex;
  double **A;
} threadData_t;

void * divisionWorker(void *arg) {
  threadData_t *td = (threadData_t *)arg;
  
  for (int j=td->jStart; j < td->N && j < (td->jStart+td->count); ++j) {
    /* A[k][j] = A[k][j] / A[k][k]; */             
    td->A[td->k][j] = td->A[td->k][j] / td->A[td->k][td->k];
  }
}

void * eliminationWorker(void *arg) {
  threadData_t *td = (threadData_t *)arg;
  
  for (int j=td->jStart; j < td->N && j < (td->jStart+td->count); ++j) {
    /* A[i][j] = A[i][j] - A[i][k] * A[k][j]; */
    td->A[td->i][j] = td->A[td->i][j] - 
                      td->A[td->i][td->k] * td->A[td->k][j];
  }

}

void print1Darray(double *X, int N) {
	for (int j = 0; j < N; ++j) {
		printf("%f\n", X[j]);
	}
	printf("\n");
}

void print2Darray(double **X, int N) {
  for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			printf("%f\t", X[i][j]);
		}
		printf("\n");
  }
	printf("\n");
}

void ge(double **A, double *b, double *y, int N, int t) {
  int i, j, k;
  /* begin */
  for (k = 0; k < N; ++k) {                     /* Outer loop */
    /* begin */
    for (j = k + 1; j < N; ++j) {
      A[k][j] = A[k][j] / A[k][k];              /* Division step */
    }

    y[k] = b[k] / A[k][k];
    A[k][k] = 1;

    for (i = k + 1; i < N; ++i) {
    /* begin */
      for (j = k + 1; j < N; ++j) {
        A[i][j] = A[i][j] - A[i][k] * A[k][j];  /* Elimination step */
      }

      b[i] = b[i] - A[i][k] * y[k];
      A[i][k] = 0;
    }
  }                                             /* Outer loop */
}

void backSub(double **U, double *x, double *y, int N) {
	int i, k;

  for (k = N - 1; k >= 0; --k) {
		x[k] = y[k];

		for (i = k - 1; i >= 0; --i) {
			y[i] = y[i] - x[k] * U[i][k];
    }
  }
}

int main(int argc, char *argv[]) {
  /* Validate arguments */
  if (argc != 3) {
    printf("Error: 2 arguments expected.\n");
    printf("Usage: ./GE <input>\n");
    exit(1);
  }

  /* Open file */
  double d;
  FILE *pFile;
  pFile = fopen(argv[1], "r");

  /* Parse matrix dimension */
  int M;
  fscanf(pFile, "%d", &M);

  /* Parse matrix A */
  double **A = malloc(sizeof(double[M]));

  for (int i=0; i<M; ++i) {
    A[i] = malloc(sizeof(double[M]));
    for (int j=0; j<M; ++j) {
      fscanf(pFile, "%lf", &d);
      A[i][j] = d;
    }
  }

  /* Parse vector b */
  double *b = malloc(sizeof(double[M]));

  for (int j=0; j<M; ++j) {
    fscanf(pFile, "%lf", &d);
    b[j] = d;
  }

  /* Close file */
  fclose(pFile);

  /* allocate vectors x and y */
  double *y = malloc(sizeof(double[M]));
  double *x = malloc(sizeof(double[M]));

  int numberOfThreads = atoi(argv[2]);

  ge(A, b, y, M, numberOfThreads);
	backSub(A, x, y, M);

  free(A);
  free(b);
  free(x);
  free(y);

  return 0;
}
