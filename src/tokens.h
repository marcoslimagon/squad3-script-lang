#ifndef __TOKENS_H__
#define __TOKENS_H__

typedef int token;

enum {
  START_PARENTHESES = 40,
  END_PARENTHESES,
  COMMA = 44,
  STMT_SEP = 59,

  UINT = 255,
  FLOAT,
  ID,

  /* start keywords */
  TRUE,
  FALSE
};

#endif /*__TOKENS_H__*/
