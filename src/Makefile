RESET = \033[0m
GREEN = \033[32m
RED = \033[31m
YELLOW = \033[33m
CYAN = \033[36m

CFLAGS = -Wall -Werror -Wextra -Wpedantic -std=c11
SHELLNAME:=$(shell uname)
ifeq ($(SHELLNAME), Linux)
  LDFLAGS = -lcheck -lsubunit -lm
else ifeq ($(SHELLNAME), Darwin)
  LDFLAGS = -lcheck
endif

DIR = .

LIB = $(wildcard $(DIR)/*.c)
OBJ = $(patsubst $(DIR)/%c, $(DIR)/%o, $(LIB))

all: s21_decimal.a test clean

s21_decimal.a: $(OBJ)
	@echo "$(YELLOW)Creating static library...$(RESET)"
	@ar rcs s21_decimal.a $(OBJ)
	@echo "$(GREEN)Static library created: s21_decimal.a$(RESET)"

%.o: %.c
	@echo "$(CYAN)Compiling $<...$(RESET)"
	@gcc $(CFLAGS) -c $< -o $@

test: s21_decimal.a
	@echo "$(CYAN)Running tests...$(RESET)"
	@gcc $(CFLAGS) -o tests/test tests/tests.c s21_decimal.a $(LDFLAGS)
	@./tests/test 2>&1 | tee test_output.log

gcov_report: CFLAGS += -coverage
gcov_report: clean test
	@echo "$(YELLOW)Generating coverage report...$(RESET)"
	@mkdir -p logs
	@lcov --capture --directory . --output-file ./logs/coverage.info
	@genhtml logs/coverage.info --output-directory ./logs/index.html
	@echo "$(GREEN)HTML report generated in logs/index.html$(RESET)"
	@open ./logs/index.html


clean:
	@echo "$(RED)Cleaning up...$(RESET)"
	@rm -rf *.a *.o ./tests/test test_check new_test *.gc* */*.gc* coverage.info out *.log
	@echo "$(GREEN)Cleanup complete$(RESET)"

html:
	@open out/index.html