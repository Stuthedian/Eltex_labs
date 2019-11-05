#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#ifndef TRY_H
#define TRY_H

void print_and_exit(char* filename, int line);

#define INVALID_PTR ((void*)-1)

#define TRY_G(comparison_operator, fail_condition) comparison_operator fail_condition ? print_and_exit(__FILE__,  __LINE__) : 0;
#define TRY_NULL  TRY_G(==, NULL)
#define TRY_INVALIED_PTR  TRY_G(==, INVALID_PTR)
#define TRY_N1  TRY_G(==, -1)
#define TRY_P1  TRY_G(==, 1)
#define TRY_ZERO  TRY_G(==, 0)
#define TRY_NEG  TRY_G(<, 0)
#define TRY_POS  TRY_G(>, 0)
#define TRY TRY_N1

#endif
