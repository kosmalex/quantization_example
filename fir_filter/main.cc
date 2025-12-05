#include <iostream>
#include <cmath>
#include "filter.h"

#ifdef INT8
  typedef int8_t qtype;
#endif

#ifdef INT16
  typedef int16_t qtype;
#endif

qtype cq[4], xq[8];
float  fir_qres_fp[8];

int main() {
  float Scoef, Sx;
  qtype Zcoef, Zx;
  quantize<qtype, 4>(cq, coeff, Scoef, Zcoef);
  quantize<qtype, 8>(xq, x    , Sx   , Zx);

  printf("============ coeff\n");
  print_vector<float, 4>("coeff", coeff, "%+1.3f", BLUE);std::cout << "\n";
  printf("============ coeff Q\n");
  print_vector<qtype, 4>("coeff", cq, "%+d", RED);std::cout << "\n";

  printf("============ x\n");
  print_vector<float, 8>("x", x, "%+1.3f", BLUE);std::cout << "\n";
  printf("============ x Q\n");
  print_vector<qtype, 8>("x", xq, "%+d", RED);std::cout << "\n";

  float out;
  for(int i = 0; i < 1; i++) {
    // We don't quantize, so set scales/zero points
    // to values such that they don't affect the output
    fir<float, 4>(x[i], coeff, 0, 0, out);
    fir_res[i] = out;
  }
  
  int64_t out_acc;
  for(int i = 0; i < 1; i++) {
    fir<qtype, 4, int64_t>(
      xq[i], cq, Zx, Zcoef, out_acc
    );
    // printf("Float cast: %f\n", float(out_acc));
    // printf("scale     : %e\n", Sx * Scoef);
    fir_qres_fp[i] = Sx * Scoef * float(out_acc);
  }

  print_vector<float, 4>("coeff", coeff, "%+1.3f", RED);std::cout << "\n";
  print_vector<float, 8>("x", x, "%+1.3f", BLUE);std::cout << "\n";
  print_vector<float, 8>("fir", fir_res, "%+1.3f", GREEN);std::cout << "\n";

  print_vector<float, 8>("q->fir", fir_qres_fp, "%+1.3f", GREEN);std::cout << "\n";
  
  return 0;
}