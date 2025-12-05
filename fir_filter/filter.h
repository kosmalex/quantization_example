float coeff[] = {
  -4.99, -3.68, 2.556, -0.413
};

float x[] = {
  0.327672, -2.81041, -4.52955, 1.78865, 1.79296, 4.34693, -1.16498, 0.19416
};

float fir_res[8];

template<typename T, int N, typename Tacc=T>
void fir(T inp, T coeff[N], T zp1, T zp2, Tacc &out) {
  static T x[N] = {0, 0, 0, 0};
  static Tacc sum = 0;
  static float s_out;

  // std::cout << "====================================\n";

  x[0] = inp;
  #pragma hls_unroll yes
  SHIFT: for (int i = N-1; i > 0; i--) {
    x[i] = x[i-1];
  }

  #pragma hls_unroll yes
  SUM_ACC: for (int i=0; i<N; i++) {
    sum += (x[i] - zp1) * (coeff[i] - zp2);
    // std::cout << sum << " <-- " << (int)((x[i] - zp1) * (coeff[i] - zp2)) << "\n";
  }
  
  out = sum;
}

template<class T, int N>
void print_vector(const char *name, T *m) {
  printf("=========== %s\n", name);
  for (int j = 0; j < N; j++) {
    printf("%+15.7f%c", (float)m[j], ",\n"[j==2]);
  }
}

template<class T, int N>
void print_vector_hls(const char *name, T *m) {
  printf("=========== %s\n", name);
  for (int j = 0; j < N; j++) {
    printf("%+15.7f%c", (float)m[j].to_double(), ",\n"[j==2]);
  }
}

float find_max(float *x, int N) {
  float max = x[0];
  for(int i = 1; i < N; i++) {
    max = (max < x[i]) ? x[i] : max;
  }
  return max;
}
float find_min(float *x, int N) {
  float min = x[0];
  for(int i = 1; i < N; i++) {
    min = (min > x[i]) ? x[i] : min;
  }
  return min;
}
float find_abs_max(float *x, int N) {
  float max = std::abs(x[0]);
  for(int i = 1; i < N; i++) {
    max = (max < std::abs(x[i])) ? std::abs(x[i]) : max;
  }
  return max;
}

template <class T>
constexpr T type_max() {
    return (T)((1ULL << (sizeof(T) * 8 - 1)) - 1);
}

template <class T>
constexpr T type_min() {
    return (T)(-type_max<T>() - 1);
}

template<class Tq, int N, int type=0>
void quantize(
  Tq x_q[N], float x_in[N], float &Sc, Tq &Zp
){
  // std::cout << "===================================\n";
  Tq q_min = type_min<Tq>(), q_max = type_max<Tq>();
  // std::cout << (int)q_min << " || " << (int)q_max << "\n";
  float a = find_min(&x_in[0], N);
  float b = find_max(&x_in[0], N);
  float r = find_abs_max(x_in, N);

  // std::cout << a << ", " << b << "\n";

  float S = type ? ( (b - a) / float(q_max - q_min) ) : (r / q_max);
  Tq Z    = type ? std::round(-a/S) + q_min : 0;
  Sc = S;
  Zp = Z;

  printf("Sc: %f\n", S);
  printf("Zp: %d\n", Z);

  for(int j = 0; j < N; j++) {
    // std::cout << x_in[j]/S << "\n";
    // std::cout << std::round(x_in[j]/S) << "\n";
    x_q[j] = std::round(x_in[j]/S) + Z;
  }
}

#include <iostream>
#include <iomanip>

// ANSI color codes
#define BLACK   "\033[0;30m"
#define RED     "\033[1;31m"
#define BLUE    "\033[1;34m"
#define GREEN   "\033[1;32m"
#define RESET   "\033[0m"

template<class T, int N>
void print_vector(const char *name, T *m, const char* fmt, const char *color) {
  std::string s(fmt); s += "%c";
  std::cout << color; 
  for (int j = 0; j < N; j++) {
    printf(s.c_str(), m[j], ",\n"[j==N-1]);
  }
  std::cout << RESET;
}