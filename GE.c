#define N 10

double A[N][N] = {
  { 35675.42, 15203.36, 46284.07, 63553.85 },
	{ 7875.82,  63482.87, 49593.29, 43800.31 },
	{ 51753.19, 30508.30, 27495.50, 51528.23 },
	{ 22011.23, 64840.39, 22470.65, 42213.57 }
};

double b[N] = { 57508.19, 39740.80, 45836.48, 56352.48 };
double y[N] = { 0.0, 0.0, 0.0, 0.0 };

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

int main(int argc, char *argv[]) {
  ge(A, b, y);

  return 0;
}
