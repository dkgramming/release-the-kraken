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

void ge(double **A, double *b, double *y, int N, int numberOfThreads) {
  int i, j, k;
  int elementsPerThread = N / numberOfThreads;
  
  pthread_t *thread = (pthread_t *)malloc(numberOfThreads * sizeof(pthread_t));
  threadData_t *arg = (threadData_t *)malloc(numberOfThreads * sizeof(threadData_t));

  /* begin */
  for (k = 0; k < N; ++k) {                     /* Outer loop */
    /* begin */
    for (int t = 0; t < numberOfThreads; ++t) {
      arg[t].jStart = (t * elementsPerThread) + (k + 1);
      arg[t].count = elementsPerThread;
      arg[t].k = k;
      arg[t].A = A;
      arg[t].N = N;
      arg[t].threadIndex = t;
      pthread_create(&thread[t], NULL, divisionWorker, (void *)(arg+t));
    }
    for (int t = 0; t < numberOfThreads; ++t) {
      pthread_join(thread[t], NULL);
    }

    y[k] = b[k] / A[k][k];
    A[k][k] = 1;

    for (i = k + 1; i < N; ++i) {
    /* begin */
      
      for (int t = 0; t < numberOfThreads; ++t) {
        arg[t].jStart = (t * elementsPerThread) + (k + 1);
        arg[t].count = elementsPerThread;
        arg[t].i = i;
        arg[t].k = k;
        arg[t].A = A;
        arg[t].N = N;
        arg[t].threadIndex = t;
        pthread_create(&thread[t], NULL, eliminationWorker, (void *)(arg+t));
      }
      for (int t = 0; t < numberOfThreads; ++t) {
        pthread_join(thread[t], NULL);
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
