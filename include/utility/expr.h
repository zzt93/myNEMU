#ifndef __EXPR_H__
#define __EXPR_H__

#include "common.h"


/*
  args: {success: use it to check whether this transformation is successful}
  return value: the value of expression
  Usage:
*/
uint32_t expr(char *e, bool *success);

#endif /* __EXPR_H__ */