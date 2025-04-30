#include "s21_decimal.h"

int s21_is_greater(s21_decimal num1, s21_decimal num2) {
  comparison_code code = COM_FALSE;
  s21_big_decimal temp1 = {0};
  s21_big_decimal temp2 = {0};
  from_dec_to_big(num1, &temp1);
  from_dec_to_big(num2, &temp2);

  code = s21_is_greater_big(temp1, temp2);

  return code;
}

int s21_is_greater_big(s21_big_decimal num1, s21_big_decimal num2) {
  comparison_code code = COM_FALSE;
  normalization(&num1, &num2);
  int sign1 = get_sign_big(num1);
  int sign2 = get_sign_big(num2);

  if (is_zero_big(num1) && is_zero_big(num2))
    code = COM_FALSE;
  else if (sign1 > sign2)
    code = COM_FALSE;
  else if (sign1 < sign2)
    code = COM_TRUE;
  else {
    for (int i = BIG_MANTISSA_SIZE - 1; i >= 0; i--) {
      if (num1.bits[i] > num2.bits[i]) {
        code = sign1 == PLUS ? COM_TRUE : COM_FALSE;
        i -= BIG_MANTISSA_SIZE;
      } else if (num1.bits[i] < num2.bits[i]) {
        code = sign1 == PLUS ? COM_FALSE : COM_TRUE;
        i -= BIG_MANTISSA_SIZE;
      }
    }
  }

  return code;
}

int s21_is_equal(s21_decimal num1, s21_decimal num2) {
  comparison_code code = COM_TRUE;
  s21_big_decimal temp1 = {0};
  s21_big_decimal temp2 = {0};
  from_dec_to_big(num1, &temp1);
  from_dec_to_big(num2, &temp2);

  code = s21_is_equal_big(temp1, temp2);

  return code;
}

int s21_is_equal_big(s21_big_decimal num1, s21_big_decimal num2) {
  comparison_code code = COM_TRUE;
  normalization(&num1, &num2);

  if (!is_zero_big(num1) || !is_zero_big(num2)) {
    for (int i = 0; i < BIG_DEC_SIZE; i++) {
      if (num1.bits[i] != num2.bits[i]) {
        code = COM_FALSE;
        i += BIG_DEC_SIZE;
      }
    }
  }

  return code;
}

int s21_is_greater_or_equal(s21_decimal num1, s21_decimal num2) {
  comparison_code code = COM_FALSE;
  code = s21_is_greater(num1, num2) || s21_is_equal(num1, num2);

  return code;
}

int s21_is_greater_or_equal_big(s21_big_decimal num1, s21_big_decimal num2) {
  comparison_code code = COM_FALSE;
  code = s21_is_greater_big(num1, num2) || s21_is_equal_big(num1, num2);

  return code;
}

int s21_is_not_equal(s21_decimal num1, s21_decimal num2) {
  return !s21_is_equal(num1, num2);
}

int s21_is_less(s21_decimal num1, s21_decimal num2) {
  return !s21_is_greater_or_equal(num1, num2);
}

int s21_is_less_or_equal(s21_decimal num1, s21_decimal num2) {
  return !s21_is_greater(num1, num2);
}
