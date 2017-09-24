#include <stdio.h>

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

  for (k = 0; k < N; ++k) {
    for (j = k + 1; j < N; ++j) {
      A[k][j] = A[k][j] / A[k][k];
    }

    y[k] = b[k] / A[k][k];
    A[k][k] = 1;

    for (i = k + 1; i < N; ++i) {
      for (j = k + 2; j < N; ++j) {
        A[i][j] = A[i][j] - A[i][k] * A[k][j];
      }

      b[i] = b[i] - A[i][k] * y[k];
      A[i][k] = 0;
    }
  }
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
  ge(A, b, y);
	print1Darray(y);
	backSub(A, x, y);
	print1Darray(x);
  return 0;
}
