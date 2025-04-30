#include "s21_decimal.h"

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  arithmetic_code code = AR_OK;
  if (result != NULL) set_zero_bits(result);

  if (check_bit3(value_1) || check_bit3(value_2) || result == NULL)
    code = AR_INPUT;
  else if (is_zero(value_2)) {
    code = AR_DIV_ZERO;
  }

  if (code == AR_OK) {
    s21_big_decimal temp_1 = {0};
    s21_big_decimal temp_2 = {0};
    s21_big_decimal temp_res = {0};
    from_dec_to_big(value_1, &temp_1);
    from_dec_to_big(value_2, &temp_2);

    code = s21_div_big(temp_1, temp_2, &temp_res);

    if (code == AR_OK) {
      code = from_big_to_dec(temp_res, result);
    }
  }

  return code;
}

int s21_div_big(s21_big_decimal num_1, s21_big_decimal num_2,
                s21_big_decimal *result) {
  arithmetic_code code = AR_OK;
  int scale = get_scale_big(num_1) - get_scale_big(num_2);
  set_scale_big(&num_1, 0);
  set_scale_big(&num_2, 0);
  int sign = get_sign_big(num_1) == get_sign_big(num_2) ? 0 : 1;
  set_sign_big(&num_1, 0);
  set_sign_big(&num_2, 0);
  s21_big_decimal remainder = {0};
  s21_big_decimal temp_res = {0};

  code = div_with_remainder(num_1, num_2, &temp_res, &remainder);
  while (!is_zero_big(remainder) && scale < 29 && temp_res.bits[3] == 0) {
    s21_add_big(shift_left_big(remainder, 1), shift_left_big(remainder, 3),
                &remainder);
    s21_big_decimal temp = {0};
    code = div_with_remainder(remainder, num_2, &temp, &remainder);
    s21_add_big(shift_left_big(temp_res, 1), shift_left_big(temp_res, 3),
                &temp_res);
    s21_add_big(temp_res, temp, &temp_res);
    scale++;
  }

  if (scale >= 0) {
    set_scale_big(&temp_res, scale);
  } else {
    while (scale < 0) {
      s21_add_big(shift_left_big(temp_res, 1), shift_left_big(temp_res, 3),
                  &temp_res);
      scale++;
    }
    set_scale_big(&temp_res, scale);
  }
  set_sign_big(&temp_res, sign);

  for (int i = 0; i < BIG_DEC_SIZE; i++) {
    result->bits[i] = temp_res.bits[i];
  }

  return code;
}

int div_with_remainder(s21_big_decimal num_1, s21_big_decimal num_2,
                       s21_big_decimal *result, s21_big_decimal *remainder) {
  arithmetic_code code = AR_OK;
  set_zero_bits_big(result);
  int shift = 0;
  s21_big_decimal temp = num_2;
  while (s21_is_greater_or_equal_big(num_1, shift_left_big(temp, 1))) {
    shift++;
    temp = shift_left_big(temp, 1);
  }
  if (s21_is_greater_or_equal_big(num_1, temp)) {
    set_bit_big(result, 0, ONE);
    s21_sub_big(num_1, temp, remainder);
  } else {
    set_bit_big(result, 0, ZERO);
    *remainder = num_1;
  }

  while (shift) {
    shift--;
    temp = num_2;
    int cycle = shift;
    while (cycle) {
      temp = shift_left_big(temp, 1);
      cycle--;
    }
    *result = shift_left_big(*result, 1);
    if (s21_is_greater_or_equal_big(*remainder, temp)) {
      set_bit_big(result, 0, ONE);
      s21_sub_big(*remainder, temp, remainder);
    } else
      set_bit_big(result, 0, ZERO);
  }

  return code;
}