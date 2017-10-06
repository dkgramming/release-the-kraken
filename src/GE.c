#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
  int threadID;
  int threadCount;
  int k;
  int N;
  double *b;
  double *y;
  double **A;
} threadData_t;

void * eliminationWorker(void *arg) {
  threadData_t *td = (threadData_t *)arg;
  int k = td->k;
  int threadID = td->threadID;
  double *b = td->b;
  double *y = td->y;
  double **A = td->A;
  int N = td->N;
  int threadCount = td->threadCount;

  for (int row = threadID + (k + 1); row < N; row += threadCount) {
    for (int col = k + 1; col < N; ++col) {
      A[row][col] = A[row][col] - A[row][k] * A[k][col];  /* Elimination step */
    }

    b[row] = b[row] - A[row][k] * y[k];
    A[row][k] = 0;
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
  int j, k;
  pthread_t *thread = (pthread_t *)malloc(t * sizeof(pthread_t));
  threadData_t *arg = (threadData_t *)malloc(t * sizeof(threadData_t));
  /* begin */
  for (k = 0; k < N; ++k) {                     /* Outer loop */
    /* begin */
    for (j = k + 1; j < N; ++j) {
      A[k][j] = A[k][j] / A[k][k];              /* Division step */
    }

    y[k] = b[k] / A[k][k];
    A[k][k] = 1;

    for (int x = 0; x < t; ++x) {
      arg[x].k = k;
      arg[x].threadID = x;
      arg[x].threadCount = t;
      arg[x].b = b;
      arg[x].y = y;
      arg[x].A = A;
      arg[x].N = N;

      pthread_create(&thread[x], NULL, eliminationWorker, (void *)(arg+x));
    }
    for (int x = 0; x < t; ++x) {
      pthread_join(thread[x], NULL);
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
