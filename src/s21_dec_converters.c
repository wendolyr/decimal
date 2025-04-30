#include "s21_decimal.h"

#define MIN_INT -2147483648
#define MAX_INT 2147483647
#define MAX_FLOAT 7.9228162514264337593543950335e+28
#define MIN_FLOAT 1e-28

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int code = ST_OK;
  unsigned int abs_src = 0;
  if (dst == NULL)
    code = ST_ERROR;
  else {
    int sign = PLUS;
    if (src < 0) {
      sign = MINUS;
      abs_src = (unsigned int)(-src);
    } else
      abs_src = src;
    set_zero_bits(dst);
    dst->bits[0] = abs_src;
    set_sign(dst, sign);
  }
  return code;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int code = ST_OK;
  float abs_src = fabs(src);
  if (dst == NULL || abs_src > MAX_FLOAT ||
      (abs_src > 0 && abs_src < MIN_FLOAT))
    code = ST_ERROR;
  else {
    char buffer[32] = "", mantissa_str[32] = "";
    sprintf(buffer, "%E", src);
    int j = 0, sign_value = 0;
    if (buffer[0] == '-') sign_value = 1;
    for (int i = 0; i < 8 + sign_value; i++) {
      if (buffer[i] != '.') mantissa_str[j++] = buffer[i];
    }
    mantissa_str[j] = '\0';
    j = 0;
    char exponent_str[32] = "";
    for (int i = strlen(buffer) - 3; i < (int)strlen(buffer); i++)
      exponent_str[j++] = buffer[i];
    int exponent = strtol(exponent_str, NULL, 10);

    int mantissa = strtol(mantissa_str, NULL, 10);
    for (int i = 0; i < 6; i++) {
      if (mantissa % 10 == 0 && exponent <= 28) {
        mantissa = mantissa / 10;
        exponent++;
      }
    }
    if (mantissa < 0) mantissa = -mantissa;
    if (mantissa != 0) exponent = exponent - 6;
    set_sign(dst, sign_value);
    dst->bits[0] = mantissa;
    s21_decimal ten = {{10, 0, 0, 0}};
    if (exponent < 0) {
      exponent = -exponent;
      double abc = mantissa;
      while (exponent > 28) {
        abc = abc / 10.0;
        exponent--;
      }
      mantissa = round(abc);
      set_scale(dst, exponent);
      dst->bits[0] = mantissa;
    } else {
      while (exponent != 0) {
        s21_mul(*dst, ten, dst);
        exponent -= 1;
      }
    }
  }
  return code;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int code = ST_OK;
  if (src.bits[1] != 0 || src.bits[2] != 0 || dst == NULL)
    code = ST_ERROR;
  else if (check_bit3(src) == 1) {
    code = ST_ERROR;
  } else {
    int scale = get_scale(src);
    int sign = get_sign(src) ? -1 : 1;
    unsigned int value = src.bits[0];
    for (int i = 0; i < scale; i++) value /= 10;
    if ((value > -MIN_INT && sign == -1) || (value > MAX_INT && sign == 1)) {
      code = ST_ERROR;
    } else {
      *dst = sign * value;
    }
  }
  return code;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  status_code code = ST_OK;
  if (!dst)
    code = ST_ERROR;
  else if (check_bit3(src)) {
    code = ST_ERROR;
    *dst = 0.0;
  } else if (is_zero(src)) {
    int sign = get_sign(src);
    if (sign == MINUS)
      *dst = -0.0;
    else
      *dst = 0.0;
  } else {
    double tmp = 0.0;
    int scale = get_scale(src);
    int sight = get_sign(src);

    for (int i = 0; i < 96; i++)
      if (get_bit(src, i)) tmp += pow(2.0, i);
    tmp /= (double)pow(10, scale);

    if (sight == MINUS) tmp *= -1.0;

    *dst = (float)tmp;
  }

  return code;
}