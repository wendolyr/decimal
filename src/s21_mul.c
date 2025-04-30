#include "s21_decimal.h"

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  arithmetic_code code = AR_OK;
  if (result != NULL) set_zero_bits(result);

  if (check_bit3(value_1) || check_bit3(value_2) || result == NULL)
    code = AR_INPUT;

  if (code == AR_OK) {
    s21_big_decimal temp_1 = {0};
    s21_big_decimal temp_2 = {0};
    s21_big_decimal temp_res = {0};
    from_dec_to_big(value_1, &temp_1);
    from_dec_to_big(value_2, &temp_2);

    code = s21_mul_big(temp_1, temp_2, &temp_res);
    if (code == AR_OK) {
      code = from_big_to_dec(temp_res, result);
    }
  }

  return code;
}

int s21_mul_big(s21_big_decimal num_1, s21_big_decimal num_2,
                s21_big_decimal *result) {
  arithmetic_code code = AR_OK;

  for (int i = 95; i >= 0; i--) {
    if (get_bit_big(num_2, i) == 1) {
      s21_add_big(num_1, *result, result);
    }
    if (i != 0) *result = shift_left_big(*result, 1);
  }

  int sign_1 = get_sign_big(num_1);
  int sign_2 = get_sign_big(num_2);
  if (sign_1 != sign_2)
    set_sign_big(result, MINUS);
  else
    set_sign_big(result, PLUS);

  int scale = get_scale_big(num_1) + get_scale_big(num_2);
  if (scale <= 28)
    set_scale_big(result, scale);
  else
    code = AR_SMALL;

  return code;
}