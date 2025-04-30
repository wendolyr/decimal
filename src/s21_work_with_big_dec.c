#include "s21_decimal.h"

int get_sign_big(s21_big_decimal num) {
  return (num.bits[BIG_DEC_SIZE - 1] & 1 << (INT_NUM_BITS - 1)) >>
         (INT_NUM_BITS - 1);
}

void set_sign_big(s21_big_decimal *num, int sign_value) {
  if (sign_value == MINUS)
    num->bits[BIG_DEC_SIZE - 1] |= MINUS << (INT_NUM_BITS - 1);
  else if (sign_value == PLUS) {
    num->bits[BIG_DEC_SIZE - 1] &= ~(MINUS << (INT_NUM_BITS - 1));
  }
}

int get_scale_big(s21_big_decimal num) {
  return (num.bits[BIG_DEC_SIZE - 1] & 255 << 16) >> 16;
}

void set_scale_big(s21_big_decimal *num, int scale_value) {
  num->bits[BIG_DEC_SIZE - 1] &= MINUS << (INT_NUM_BITS - 1);
  num->bits[BIG_DEC_SIZE - 1] |= scale_value << 16;
}

int get_bit_big(s21_big_decimal num, int index) {
  bits bit = ZERO;
  for (int i = 0; i < BIG_DEC_SIZE; i++) {
    if (index >= INT_NUM_BITS) {
      index -= INT_NUM_BITS;
    } else {
      bit = (num.bits[i] & 1 << index) >> index;
      i += BIG_DEC_SIZE;
    }
  }
  return bit;
}

void set_bit_big(s21_big_decimal *num, int bit, unsigned value) {
  for (int i = 0; i < BIG_DEC_SIZE; i++) {
    if (bit >= INT_NUM_BITS)
      bit -= INT_NUM_BITS;
    else {
      if (value) {
        num->bits[i] |= value << bit;
      } else {
        num->bits[i] &= ~(1 << bit);
      }
      i += BIG_DEC_SIZE;
    }
  }
}

s21_big_decimal shift_left_big(s21_big_decimal num, int shift) {
  unsigned int mem = 0;
  for (int i = 0; i < BIG_DEC_SIZE - 1; i++) {
    unsigned int temp = num.bits[i];
    num.bits[i] <<= shift;
    num.bits[i] |= mem;
    mem = temp >> (INT_NUM_BITS - shift);
  }
  return num;
}

void print_decimal_big(s21_big_decimal num) {
  printf("Big: ");
  for (int i = 127; i >= 0; i--) {
    if (i == 95 || i == 63 || i == 31) printf(" ");
    printf("%d", get_bit_big(num, i));
  }
  printf("\n");
}

void from_dec_to_big(s21_decimal num, s21_big_decimal *res) {
  for (int i = 0; i < DEC_SIZE - 1; i++) {
    res->bits[i] = num.bits[i];
  }
  for (int i = DEC_SIZE - 1; i < BIG_DEC_SIZE - 1; i++) {
    res->bits[i] = 0;
  }
  res->bits[BIG_DEC_SIZE - 1] = num.bits[DEC_SIZE - 1];
}

int from_big_to_dec(s21_big_decimal num, s21_decimal *res) {
  int code = AR_OK;
  int scale = get_scale_big(num);
  int sign = get_sign_big(num);
  s21_big_decimal temp = {0};
  s21_big_decimal rem = {0};
  int shift = 0;
  set_scale_big(&num, 0);
  set_sign_big(&num, 0);
  int bank_flag = 0;
  while (num.bits[3] || num.bits[4] || num.bits[5] || num.bits[6] ||
         scale > 28) {
    temp = num;
    if (!s21_is_equal_big(rem, (s21_big_decimal){0})) bank_flag = 1;
    div_with_remainder(num, create_ten_big(), &num, &rem);
    scale--;
    shift++;
  }

  if (scale >= 0 && shift) {
    bank_rounding(temp, &num, bank_flag);
  } else if (scale < 0) {
    code = sign ? AR_SMALL : AR_BIG;
  }

  for (int i = DEC_SIZE - 1; i < BIG_DEC_SIZE - 1 && code == AR_OK; i++) {
    if (num.bits[i] != 0) {
      code = sign ? AR_SMALL : AR_BIG;
    }
  }

  if (code == AR_OK) {
    for (int i = 0; i < DEC_SIZE - 1; i++) {
      res->bits[i] = num.bits[i];
    }
    set_sign(res, sign);
    set_scale(res, scale);
  }

  return code;
}

void normalization(s21_big_decimal *num_1, s21_big_decimal *num_2) {
  int scale_1 = get_scale_big(*num_1);
  int scale_2 = get_scale_big(*num_2);
  s21_big_decimal res = {0};
  while (scale_1 > scale_2) {
    s21_add_big(shift_left_big(*num_2, 1), shift_left_big(*num_2, 3), &res);
    *num_2 = res;
    scale_2++;
    set_scale_big(num_2, scale_2);
  }
  while (scale_2 > scale_1) {
    s21_add_big(shift_left_big(*num_1, 1), shift_left_big(*num_1, 3), &res);
    *num_1 = res;
    scale_1++;
    set_scale_big(num_1, scale_1);
  }
}

void set_zero_bits_big(s21_big_decimal *num) {
  for (int i = 0; i < BIG_DEC_SIZE; i++) {
    num->bits[i] = 0;
  }
}

int is_zero_big(s21_big_decimal num) {
  int code = 1;
  for (int i = 0; i < BIG_DEC_SIZE - 1 && code == 1; i++) {
    if (num.bits[i] != 0) code = 0;
  }

  return code;
}

s21_big_decimal create_ten_big() {
  s21_big_decimal ten = {0};
  ten.bits[0] = 10;

  return ten;
}