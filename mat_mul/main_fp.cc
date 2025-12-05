#include <iostream>
#include <cmath>
#include "matrix.h"

#include "ac_std_float.h"

#ifdef BF16
  typedef ac_std_float<16,8> hls_float_t;
#endif
#ifdef FP8_E5M2
  typedef ac_std_float<8,5> hls_float_t;
#endif
#ifdef FP8_E4M3
  typedef ac_std_float<8,4> hls_float_t;
#endif
#ifdef FP32
  typedef ac_std_float<32,8> hls_float_t;
#endif

hls_float_t a_fp_hls[3][3], b_fp_hls[3][3], c_fp_hls[3][3];
float   c_fp_hls_fp[3][3];

int main() {
  // print_matrix("a", a);
  // print_matrix("b", b);

  float *a_ptr = &a[0][0], *b_ptr = &b[0][0];
  for(int i = 0; i < 3; i++)
  for(int j = 0; j < 3; j++) {
    a_fp_hls[i][j] = (hls_float_t)*a_ptr; a_ptr++;
    b_fp_hls[i][j] = (hls_float_t)*b_ptr; b_ptr++;
  }
  
  mat_mul(c, a, b);
  mat_mul_hls_fp<hls_float_t>(c_fp_hls, a_fp_hls, b_fp_hls);
  
  printMatrixMultiplication(a, b, c);

  print_matrix("a", a);
  print_matrix_hls("a_fp_hls", a_fp_hls);
  print_matrix("b", b);
  print_matrix_hls("b_fp_hls", b_fp_hls);

  // print_matrix("c", c);

  float err[3][3];
  for(int i = 0; i < 3; i++)
    for(int j = 0; j < 3; j++) {
      c_fp_hls_fp[i][j] = (float)c_fp_hls[i][j].to_double();
      err[i][j] = c[i][j] - c_fp_hls_fp[i][j];
    }

  print_matrix("err", err);

  printTriangularMatrices(c_fp_hls_fp, c, err);

  return 0;
}