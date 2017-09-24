#include <stdio.h>
#include <stdlib.h>

#define N 3

double A[N][N] = {
  { 1, 1, -1 },
  { 0, 1, 3 },
  { -1, 0, -2 }
};

double b[N] = { 9, 3, 2 };
double y[N];
double x[N];

void print1Darray(double X[N]) {
	for (int j = 0; j < N; ++j) {
		printf("%f\n", X[j]);
	}
	printf("\n");
}

void print2Darray(double X[N][N]) {
  for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			printf("%f\t", X[i][j]);
		}
		printf("\n");
  }
	printf("\n");
}

void ge(double A[N][N], double b[N], double y[N]) {
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

void backSub(double U[N][N], double x[N], double y[N]) {
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
  if (argc != 5) {
    printf("Error: 4 arguments expected.\n");
    printf("Usage: ./GE <N> <A> <b> <tc>\n");
    printf("\tN - dimension of matrix A and vector b\n");
    printf("\tA - matrix A\n");
    printf("\tb - vector b\n");
    printf("\ttc - number of threads\n");
    exit(1);
  }

  /* Open file */
  double d;
  FILE *pFile;

  /* Parse matrix A */
  int M = atoi(argv[1]);
  double (*A)[M] = malloc(sizeof(double[M][M]));
  pFile = fopen(argv[2], "r");

  for (int i=0; i<M; ++i) {
    for (int j=0; j<M; ++j) {
      fscanf(pFile, "%lf", &d);
      printf("A[%d][%d] = %f\n", i, j, d);
      A[i][j] = d;
    }
  }

  ge(A, b, y);
	print1Darray(y);
	backSub(A, x, y);
	print1Darray(x);
  return 0;
}
