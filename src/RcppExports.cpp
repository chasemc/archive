// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <Rcpp.h>

using namespace Rcpp;

// archive_metadata
Rcpp::List archive_metadata(const std::string& path);
RcppExport SEXP archive_archive_metadata(SEXP pathSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const std::string& >::type path(pathSEXP);
    rcpp_result_gen = Rcpp::wrap(archive_metadata(path));
    return rcpp_result_gen;
END_RCPP
}
// read_connection
SEXP read_connection(const std::string& archive_filename, const std::string& filename, size_t sz);
RcppExport SEXP archive_read_connection(SEXP archive_filenameSEXP, SEXP filenameSEXP, SEXP szSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const std::string& >::type archive_filename(archive_filenameSEXP);
    Rcpp::traits::input_parameter< const std::string& >::type filename(filenameSEXP);
    Rcpp::traits::input_parameter< size_t >::type sz(szSEXP);
    rcpp_result_gen = Rcpp::wrap(read_connection(archive_filename, filename, sz));
    return rcpp_result_gen;
END_RCPP
}
// write_connection
SEXP write_connection(const std::string& archive_filename, const std::string& filename, size_t sz);
RcppExport SEXP archive_write_connection(SEXP archive_filenameSEXP, SEXP filenameSEXP, SEXP szSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const std::string& >::type archive_filename(archive_filenameSEXP);
    Rcpp::traits::input_parameter< const std::string& >::type filename(filenameSEXP);
    Rcpp::traits::input_parameter< size_t >::type sz(szSEXP);
    rcpp_result_gen = Rcpp::wrap(write_connection(archive_filename, filename, sz));
    return rcpp_result_gen;
END_RCPP
}
// write_files
SEXP write_files(const std::string& archive_filename, Rcpp::CharacterVector files, size_t sz);
RcppExport SEXP archive_write_files(SEXP archive_filenameSEXP, SEXP filesSEXP, SEXP szSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const std::string& >::type archive_filename(archive_filenameSEXP);
    Rcpp::traits::input_parameter< Rcpp::CharacterVector >::type files(filesSEXP);
    Rcpp::traits::input_parameter< size_t >::type sz(szSEXP);
    rcpp_result_gen = Rcpp::wrap(write_files(archive_filename, files, sz));
    return rcpp_result_gen;
END_RCPP
}
