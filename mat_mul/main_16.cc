#include <iostream>
#include <cmath>
#include "matrix.h"

int16_t aq[3][3], bq[3][3];

int main() {
  float Sa, Sb;
  int16_t Za;
  quantize_int16  (aq, a, Sa, Za);
  quantize_int16_u(bq, b, Sb);

  print_matrix("a", a);
  print_matrix("aq", aq);
  print_matrix("b", b);
  print_matrix("bq", bq);
  
  mat_mul(c, a, b);
  
  int64_t c_acc[3][3];
  float   c_acc_fp[3][3];
  mat_mul_q(c_acc, aq, Za, bq);
  print_matrix("c_acc", c_acc);

  for(int i = 0; i < 3; i++)
    for(int j = 0; j < 3; j++) {
      c_acc_fp[i][j] = Sa * Sb * (float)c_acc[i][j];
    }


  print_matrix("c", c);
  print_matrix("c_bar", c_acc_fp);

  float err[3][3];
  for(int i = 0; i < 3; i++)
    for(int j = 0; j < 3; j++) {
      err[i][j] = c[i][j] - c_acc_fp[i][j];
    }

  print_matrix("err", err);
  
  printMatrixMultiplication(a, b, c);
  printTriangularMatrices(c_acc_fp, c, err);

  return 0;
}