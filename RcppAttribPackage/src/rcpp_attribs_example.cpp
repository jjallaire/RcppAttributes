

#include <Rcpp.h>

#include <vector>

// [[Rcpp::import_package(Matrix)]]


// [[Rcpp::export]]
int foo(int x, int y) {
  
  return x + y;
}

extern int foo(int x, int y);

RcppExport SEXP fooCall(SEXP x, SEXP y) {
BEGIN_RCPP
  Rcpp::RNGScope scope;
  int res = foo(Rcpp::as<int>(x), Rcpp::as<int>(y));
  return Rcpp::wrap(res);
END_RCPP
}

/*
foo <- function(x, y){
  .Call( "fooCall", PACKAGE = "RcppAttribPackage", x, y)
}
*/


// [[Rcpp::export]]
std::vector<int> times4(const std::vector<int>& src) {
  return src;
}

extern std::vector<int> times4(const std::vector<int>& src);

RcppExport SEXP times4Call(SEXP src) {
BEGIN_RCPP
  Rcpp::RNGScope scope;
  std::vector<int> res = times4(Rcpp::as<std::vector<int> >(src));
  return Rcpp::wrap(res);
END_RCPP
}



