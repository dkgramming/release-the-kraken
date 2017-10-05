#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
  int i;
  int j;
  int k;
  double **A;
} threadData_t;

void * divisionWorker(void *arg) {
  threadData_t *td = (threadData_t *)arg;
  
  /* A[k][j] = A[k][j] / A[k][k]; */             
  td->A[td->k][td->j] = td->A[td->k][td->j] / td->A[td->k][td->k];
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

void ge(double **A, double *b, double *y, int N) {
  int i, j, k;
  pthread_t *thread = (pthread_t *)malloc(N * sizeof(pthread_t));
  threadData_t *arg = (threadData_t *)malloc(N * sizeof(pthread_t));

  /* begin */
  for (k = 0; k < N; ++k) {                     /* Outer loop */
    /* begin */
    for (j = k + 1; j < N; ++j) {
      arg[j].i = i;
      arg[j].j = j;
      arg[j].k = k;
      arg[j].A = A;
      pthread_create(&thread[j], NULL, divisionWorker, (void *)(arg+j));
    }
    for (j = k + 1; j < N; ++j) {
      pthread_join(thread[j], NULL);
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
    printf("Usage: ./GE <input> <thread_count>\n");
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

  ge(A, b, y, M);
	backSub(A, x, y, M);
	print1Darray(x, M);

  free(A);
  free(b);
  free(x);
  free(y);

  return 0;
}
