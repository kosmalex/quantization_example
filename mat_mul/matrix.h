float a[3][3] = {
  {-4.99  , 2.556 , 0.3276},
  {-4.5295, 1.7929, -1.1649},
  {3.3096 , -4.465, 1.71149},
};

float b[3][3] = {
  {-3.6846, -0.4134, -2.8104},
  {1.78865, 4.3469, 0.19415},
  {-4.6543, 0.297, -4.923},
};

float c[3][3];

void mat_mul(float c[3][3], float a[3][3], float b[3][3]) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      float sum = 0.0f;
      for (int k = 0; k < 3; k++) {
        sum += a[i][k] * b[k][j];
      }
      c[i][j] = sum;
    }
  }
}

template<class T>
void mat_mul_hls_fp(T c[3][3], T a[3][3], T b[3][3]) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      T sum = T(0.0f);
      for (int k = 0; k < 3; k++) {
        sum += a[i][k] * b[k][j];
      }
      c[i][j] = sum;
    }
  }
}

void mat_mul_q(int32_t c[3][3], int8_t a[3][3], int8_t Za, int8_t b[3][3]) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      int32_t sum = 0, sum_b = 0;
      for (int k = 0; k < 3; k++) {
        sum   += a[i][k] * b[k][j];
        sum_b += b[k][j];
      }
      c[i][j] = sum - Za * sum_b;
    }
  }
}

void mat_mul_q(int64_t c[3][3], int16_t a[3][3], int16_t Za, int16_t b[3][3]) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      int64_t sum = 0, sum_b = 0;
      for (int k = 0; k < 3; k++) {
        sum   += a[i][k] * b[k][j];
        sum_b += b[k][j];
      }
      c[i][j] = sum - Za * sum_b;
    }
  }
}

template<class T>
void print_matrix(const char *name, T m[3][3]) {
  printf("=========== %s\n", name);
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      printf("%+15.7f%c", (float)m[i][j], ",\n"[j==2]);
    }
  }
}

template<class T>
void print_matrix_hls(const char *name, T m[3][3]) {
  printf("=========== %s\n", name);
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      printf("%+15.7f%c", (float)m[i][j].to_double(), ",\n"[j==2]);
    }
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

void quantize_int8(
  int8_t x_q[3][3], float x_in[3][3], float &Sc, int8_t &Zp
){
  int8_t q_min = -128, q_max = 127;
  float a = find_min(&x_in[0][0], 9);
  float b = find_max(&x_in[0][0], 9);

  float  S = (b - a) / float(q_max - q_min);
  int8_t Z = std::round(-a/S) + q_min;
  Sc = S;
  Zp = Z;

  printf("Sc: %f\n", S);
  printf("Zp: %d\n", Z);

  for(int i = 0; i < 3; i++)
    for(int j = 0; j < 3; j++) {
      x_q[i][j] = std::round(x_in[i][j]/S) + Z;
    }
}

void quantize_int8_u(int8_t x_q[3][3], float x_in[3][3], float &Sc) {
  int8_t q_min = -127, q_max = 127;
  float r = find_abs_max(&x_in[0][0], 9);

  // std::cout << r << "\n";
  float S = 2*r / float(q_max - q_min);
  Sc = S;

  printf("Sc: %f\n", S);

  for(int i = 0; i < 3; i++)
    for(int j = 0; j < 3; j++) {
      // std::cout << x_in[i][j] << " > " << x_in[i][j]/S << "\n";
      x_q[i][j] = std::round(x_in[i][j]/S);
    }
}

void quantize_int16(
  int16_t x_q[3][3], float x_in[3][3], float &Sc, int16_t &Zp
){
  int16_t q_min = -(1 << 15), q_max = (1 << 15) - 1;
  float a = find_min(&x_in[0][0], 9);
  float b = find_max(&x_in[0][0], 9);

  float  S = (b - a) / float(q_max - q_min);
  int16_t Z = std::round(-a/S) + q_min;
  Sc = S;
  Zp = Z;

  printf("Sc: %f\n", S);
  printf("Zp: %d\n", Z);

  for(int i = 0; i < 3; i++)
    for(int j = 0; j < 3; j++) {
      x_q[i][j] = std::round(x_in[i][j]/S) + Z;
    }
}

void quantize_int16_u(int16_t x_q[3][3], float x_in[3][3], float &Sc) {
  int16_t q_min = -(1 << 15) + 1, q_max = (1 << 15) - 1;
  float r = find_abs_max(&x_in[0][0], 9);

  std::cout << r << "\n";
  float S = 2*r / float(q_max - q_min);
  Sc = S;

  printf("Sc: %f\n", S);

  for(int i = 0; i < 3; i++)
    for(int j = 0; j < 3; j++) {
      // std::cout << x_in[i][j] << " > " << x_in[i][j]/S << "\n";
      x_q[i][j] = std::round(x_in[i][j]/S);
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

void printMatrixMultiplication(float A[3][3], float B[3][3], float C[3][3])
{
    for (int i = 0; i < 3; i++)
    {
        /* ========== MATRIX A ========== */
        std::cout << RED;

        if (i == 0) std::cout << "┌";
        else if (i == 2) std::cout << "└";
        else std::cout << "│";

        for (int j = 0; j < 3; j++)
          printf("%+8.4f", A[i][j]);

        if (i == 0) std::cout << " ┐";
        else if (i == 2) std::cout << " ┘";
        else std::cout << " │";

        std::cout << RESET;

        /* ========== MULTIPLY SIGN ========== */
        if (i == 1)
            std::cout << " x ";
        else
            std::cout << "   ";

        /* ========== MATRIX B ========== */
        std::cout << BLUE;

        if (i == 0) std::cout << "┌";
        else if (i == 2) std::cout << "└";
        else std::cout << "│";

        for (int j = 0; j < 3; j++)
          printf("%+8.4f", B[i][j]);

        if (i == 0) std::cout << " ┐";
        else if (i == 2) std::cout << " ┘";
        else std::cout << " │";

        std::cout << RESET;

        /* ========== EQUAL SIGN ========== */
        if (i == 1)
            std::cout << " = ";
        else
            std::cout << "   ";

        /* ========== MATRIX C ========== */
        std::cout << GREEN;

        if (i == 0) std::cout << "┌";
        else if (i == 2) std::cout << "└";
        else std::cout << "│";

        for (int j = 0; j < 3; j++)
          printf("%+10.4f", C[i][j]);

        if (i == 0) std::cout << " ┐";
        else if (i == 2) std::cout << " ┘";
        else std::cout << " │";

        std::cout << RESET << std::endl;
    }
}

void printTriangularMatrices(float A[3][3], float B[3][3], float C[3][3])
{
    // ===== Top row: Matrices A and B =====
    for (int i = 0; i < 3; i++)
    {
        // Matrix A
        if (i == 0) std::cout << BLACK << "┌";
        else if (i == 2) std::cout << BLACK << "└";
        else std::cout << BLACK << "│";

        for (int j = 0; j < 3; j++)
          printf("%+10.4f", A[i][j]);

        if (i == 0) std::cout << " ┐" << RESET;
        else if (i == 2) std::cout << " ┘" << RESET;
        else std::cout << " │" << RESET;

        std::cout << std::endl;
    }

    // ===== Bottom row: Matrix C (centered) =====
    for (int i = 0; i < 3; i++)
    {
        if (i == 0) std::cout << RED << "┌";
        else if (i == 2) std::cout << RED << "└";
        else std::cout << RED << "│";

        for (int j = 0; j < 3; j++)
          printf("%+10.4f", C[i][j]);

        if (i == 0) std::cout << " ┐" << RESET;
        else if (i == 2) std::cout << RED << " ┘" << RESET;
        else std::cout << " │" << RESET;

        std::cout << std::endl;
    }
}
