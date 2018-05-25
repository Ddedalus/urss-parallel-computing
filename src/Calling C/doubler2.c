#include <R.h>
#include <Rdefines.h>
SEXP double_me2(SEXP x) {
  // Doubles the value of the first integer element of the SEXP input
  SEXP result;
  PROTECT(result = NEW_INTEGER(1)); // i.e., a scalar quantity
  INTEGER(result)[0] = INTEGER(x)[0] * 2;
  UNPROTECT(1); // Release the one item that was protected
  return result;
}