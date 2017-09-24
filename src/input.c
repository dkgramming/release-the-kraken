#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

float A[16384][16384],B[16384],X[16384];
int N=4;

void initialize_inputs(float A[16384][16384],float B[16384],float X[16384]) {
  int row, col;

  for (col = 0; col < N; col++) {
    for (row = 0; row < N; row++) {
      A[row][col] = (float)rand() / 32768.0;
    }
    B[col] = (float)rand() / 32768.0;
    X[col] = 0.0;
  }

}

void print_inputs() {
  int row, col;

  if (N <= 16384) {
    printf("%d\t\n", N);
    for (row = 0; row < N; row++) {
      for (col = 0; col < N; col++) {
        printf("%5.2f%s", A[row][col], (col < N-1) ? " " : "\t\n");
      }
    }
    for (col = 0; col < N; col++) {
      printf("%5.2f%s", B[col], (col < N-1) ? " " : "\t\n");
    }
  }
}

int main(int argc, char *argv[]) {
  int seed = time(NULL);
  srand(seed);
  if (argc > 1) {
    N = atoi(argv[1]);
  }
  initialize_inputs(A,B,X);
  print_inputs();
}
