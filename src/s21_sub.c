#include "s21_decimal.h"

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
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
    normalization(&temp_1, &temp_2);

    int sign_1 = get_sign(value_1);
    int sign_2 = get_sign(value_2);
    set_sign(&value_1, PLUS);
    set_sign(&value_2, PLUS);
    if ((sign_1 == PLUS && sign_2 == MINUS) ||
        (sign_1 == MINUS && sign_2 == PLUS)) {
      code = s21_add_big(temp_1, temp_2, &temp_res);
    } else {
      if (s21_is_greater(value_1, value_2)) {
        code = s21_sub_big(temp_1, temp_2, &temp_res);
        set_sign_big(&temp_res, sign_1);
      } else if (s21_is_less(value_1, value_2)) {
        code = s21_sub_big(temp_2, temp_1, &temp_res);
        set_sign_big(&temp_res, !sign_2);
      }
    }

    if (code == AR_OK) code = from_big_to_dec(temp_res, result);
  }

  return code;
}

int s21_sub_big(s21_big_decimal num_1, s21_big_decimal num_2,
                s21_big_decimal *result) {
  arithmetic_code code = AR_OK;
  set_zero_bits_big(result);

  int mem = 0;
  int borrow_scale = 0;

  for (int i = 0; i < 224; i++) {
    if (borrow_scale == 1) {
      mem = -1;
    } else if (borrow_scale > 1) {
      mem = 1;
    } else if (borrow_scale == 0) {
      mem = 0;
    }

    int minuend = get_bit_big(num_1, i) + mem;
    int subtrahend = get_bit_big(num_2, i);

    if (borrow_scale > 0) borrow_scale--;

    if (minuend < subtrahend) {
      int j = i + 1;
      borrow_scale++;
      while (get_bit_big(num_1, j) == 0 && j < 224) {
        j++;
        borrow_scale++;
      }
      minuend += 2;
    }

    int difference = minuend - subtrahend;
    set_bit_big(result, i, difference);
  }

  set_scale_big(result, get_scale_big(num_1));

  return code;
}