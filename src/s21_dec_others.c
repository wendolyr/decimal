#include "s21_decimal.h"

int s21_negate(s21_decimal value, s21_decimal *result) {
  status_code code = ST_OK;
  if (check_bit3(value) || result == NULL)
    code = ST_ERROR;
  else {
    set_zero_bits(result);
    int sign = !get_sign(value);
    *result = value;
    set_sign(result, sign);
  }

  return code;
}

int s21_truncate(s21_decimal value, s21_decimal *result) {
  status_code code = ST_OK;
  if (check_bit3(value) || result == NULL)
    code = ST_ERROR;
  else {
    set_zero_bits(result);
    s21_big_decimal temp_val = {0};
    from_dec_to_big(value, &temp_val);
    s21_truncate_big(temp_val, &temp_val);
    from_big_to_dec(temp_val, result);
  }

  return code;
}

void s21_truncate_big(s21_big_decimal value, s21_big_decimal *result) {
  set_zero_bits_big(result);
  s21_big_decimal rem = {0};

  int sign = get_sign_big(value);
  int scale = get_scale_big(value);
  if (scale == 0) {
    *result = value;
  } else {
    set_sign_big(&value, PLUS);
    while (scale) {
      set_scale_big(&value, 0);
      div_with_remainder(value, create_ten_big(), &value, &rem);
      scale--;
    }
    *result = value;
    set_scale_big(result, 0);
    set_sign_big(result, sign);
  }
}

void bank_rounding(s21_big_decimal value, s21_big_decimal *result,
                   int bank_flag) {
  int count = 1;
  int remainder = 0;
  for (int i = 0; i < 224; i++) {
    if (get_bit_big(value, i) == 1) {
      remainder = (remainder + count) % 100;
    }
    count = (count * 2) % 100;
  }

  int last_digit = remainder % 10;
  int penult_digit = remainder / 10;

  set_scale_big(&value, 1);
  s21_big_decimal one = {0};
  one.bits[0] = 1;

  if (last_digit > 5 || (last_digit == 5 && bank_flag)) {
    s21_truncate_big(value, result);
    s21_add_big(*result, one, result);
  } else if (last_digit < 5) {
    s21_truncate_big(value, result);
  } else {
    if (penult_digit % 2 == 0) {
      s21_truncate_big(value, result);
    } else {
      s21_truncate_big(value, result);
      s21_add_big(*result, one, result);
    }
  }
}

int s21_round(s21_decimal value, s21_decimal *result) {
  status_code code = ST_OK;
  if (check_bit3(value) || result == NULL)
    code = ST_ERROR;
  else {
    set_zero_bits(result);
    s21_decimal integer = {{0}};
    s21_decimal fract = {{0}};

    s21_decimal half = {{5, 0, 0, 0}};
    set_scale(&half, 1);

    int sign = get_sign(value);
    if (sign == MINUS) set_sign(&value, PLUS);

    s21_truncate(value, &integer);
    s21_sub(value, integer, &fract);

    if (s21_is_greater_or_equal(fract, half))
      s21_add(integer, (s21_decimal){{1, 0, 0, 0}}, result);
    else {
      *result = integer;
    }
    set_sign(result, sign);
  }
  return code;
}

int s21_floor(s21_decimal value, s21_decimal *result) {
  status_code code = ST_OK;
  if (check_bit3(value) || result == NULL)
    code = ST_ERROR;
  else {
    set_zero_bits(result);
    int sign = get_sign(value);

    s21_truncate(value, result);
    if (sign == MINUS && s21_is_not_equal(value, *result)) {
      s21_decimal one = {{1, 0, 0, 0}};
      set_sign(&one, MINUS);
      s21_add(*result, one, result);
    }
  }

  return code;
}