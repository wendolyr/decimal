#include "s21_decimal.h"

int get_sign(s21_decimal num) {
  return (num.bits[DEC_SIZE - 1] & 1 << (INT_NUM_BITS - 1)) >>
         (INT_NUM_BITS - 1);
}

void set_sign(s21_decimal *num, int sign_value) {
  if (sign_value == MINUS)
    num->bits[DEC_SIZE - 1] |= MINUS << (INT_NUM_BITS - 1);
  else if (sign_value == PLUS)
    num->bits[DEC_SIZE - 1] &= ~(MINUS << (INT_NUM_BITS - 1));
}

int get_scale(s21_decimal num) { return (num.bits[3] & 255 << 16) >> 16; }

void set_scale(s21_decimal *num, int scale_value) {
  num->bits[DEC_SIZE - 1] |= scale_value << 16;
}

int get_bit(s21_decimal num, int index) {
  bits temp = ZERO;
  for (int i = 0; i < DEC_SIZE; i++) {
    if (index >= INT_NUM_BITS) {
      index -= INT_NUM_BITS;
    } else {
      temp = (num.bits[i] & 1 << index) >> index;
      i += DEC_SIZE;
    }
  }
  return temp;
}

void set_bit(s21_decimal *num, int bit, unsigned value) {
  for (int i = 0; i < DEC_SIZE; i++) {
    if (bit >= INT_NUM_BITS)
      bit -= INT_NUM_BITS;
    else {
      if (value) {
        num->bits[i] |= value << bit;
      } else {
        num->bits[i] &= ~(1 << bit);
      }
      i += DEC_SIZE;
    }
  }
}

void print_decimal(s21_decimal num) {
  for (int i = 127; i >= 0; i--) {
    if (i == 95 || i == 63 || i == 31) printf(" ");
    if (i == 119 || i == 111) printf("-");
    printf("%d", get_bit(num, i));
  }
  printf("\n");
}

int check_bit3(s21_decimal num) {
  int code = 0;
  if (get_scale(num) > 28) code = 1;
  for (int i = 96; i < 112 && code == 0; i++) {
    if (get_bit(num, i) != 0) code = 1;
  }
  for (int i = 120; i < 127 && code == 0; i++) {
    if (get_bit(num, i) != 0) code = 1;
  }

  return code;
}

void set_zero_bits(s21_decimal *num) {
  for (int i = 0; i < DEC_SIZE; i++) {
    num->bits[i] = ZERO;
  }
}

int is_zero(s21_decimal num) {
  int code = 1;
  for (int i = 0; i < DEC_SIZE - 1 && code == 1; i++) {
    if (num.bits[i] != 0) code = 0;
  }

  return code;
}