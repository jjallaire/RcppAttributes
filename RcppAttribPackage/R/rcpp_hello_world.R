
rcpp_hello_world <- function(){
	.Call( "rcpp_hello_world", PACKAGE = "RcppAttribPackage" )
}


foo <- function(x, y){
  .Call( "fooCall", PACKAGE = "RcppAttribPackage", x, y)
}


times4 <- function(src){
  .Call( "times4Call", PACKAGE = "RcppAttribPackage", src)
}