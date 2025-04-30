#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H

#define DEC_SIZE 4
#define BIG_DEC_SIZE 8
#define MANTISSA_SIZE 3
#define BIG_MANTISSA_SIZE 7
#define UINT_MAX 4294967295

#define INT_NUM_BITS 32

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  unsigned int bits[DEC_SIZE];
} s21_decimal;

typedef struct {
  unsigned int bits[BIG_DEC_SIZE];
} s21_big_decimal;

typedef enum {
  PLUS = 0,
  MINUS = 1,
} sign;

typedef enum {
  ZERO = 0,
  ONE = 1,
} bits;

typedef enum {
  AR_OK = 0,
  AR_BIG = 1,
  AR_SMALL = 2,
  AR_DIV_ZERO = 3,
  AR_INPUT = 4
} arithmetic_code;

typedef enum { COM_FALSE = 0, COM_TRUE = 1 } comparison_code;

typedef enum { ST_OK = 0, ST_ERROR = 1 } status_code;

int get_sign(s21_decimal num);
void set_sign(s21_decimal *num, int sign_value);
int get_scale(s21_decimal num);
void set_scale(s21_decimal *num, int scale_value);
int get_bit(s21_decimal num, int bit);
void set_bit(s21_decimal *num, int bit, unsigned value);
void print_decimal(s21_decimal num);
int check_bit3(s21_decimal num);
void set_zero_bits(s21_decimal *num);
int is_zero(s21_decimal num);
int get_scale_big(s21_big_decimal num);
void set_scale_big(s21_big_decimal *num, int scale_value);

s21_big_decimal shift_left_big(s21_big_decimal num, int shift);
void from_dec_to_big(s21_decimal num, s21_big_decimal *res);
int from_big_to_dec(s21_big_decimal num, s21_decimal *res);
int get_bit_big(s21_big_decimal num, int index);
void set_bit_big(s21_big_decimal *num, int bit, unsigned value);
void print_decimal_big(s21_big_decimal num);
int get_sign_big(s21_big_decimal num);
void set_sign_big(s21_big_decimal *num, int sign_value);
void set_zero_bits_big(s21_big_decimal *num);
int is_zero_big(s21_big_decimal num);
s21_big_decimal create_ten_big();

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_add_big(s21_big_decimal num_1, s21_big_decimal num_2,
                s21_big_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub_big(s21_big_decimal num_1, s21_big_decimal num_2,
                s21_big_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul_big(s21_big_decimal num_1, s21_big_decimal num_2,
                s21_big_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div_big(s21_big_decimal num_1, s21_big_decimal num_2,
                s21_big_decimal *result);
int div_with_remainder(s21_big_decimal num_1, s21_big_decimal num_2,
                       s21_big_decimal *result, s21_big_decimal *remainder);

void normalization(s21_big_decimal *num_1, s21_big_decimal *num_2);

int s21_is_less(s21_decimal, s21_decimal);
int s21_is_less_or_equal(s21_decimal, s21_decimal);
int s21_is_greater(s21_decimal num1, s21_decimal num2);
int s21_is_greater_big(s21_big_decimal num1, s21_big_decimal num2);
int s21_is_greater_or_equal(s21_decimal num1, s21_decimal num2);
int s21_is_greater_or_equal_big(s21_big_decimal num1, s21_big_decimal num2);
int s21_is_equal(s21_decimal num1, s21_decimal num2);
int s21_is_equal_big(s21_big_decimal num1, s21_big_decimal num2);
int s21_is_not_equal(s21_decimal num1, s21_decimal num2);

int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
void s21_truncate_big(s21_big_decimal value, s21_big_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);
void bank_rounding(s21_big_decimal value, s21_big_decimal *result,
                   int bank_flag);

#endif