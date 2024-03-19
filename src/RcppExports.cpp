// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <RcppArmadillo.h>
#include <RcppGSL.h>
#include <Rcpp.h>

using namespace Rcpp;

#ifdef RCPP_USE_GLOBAL_ROSTREAM
Rcpp::Rostream<true>&  Rcpp::Rcout = Rcpp::Rcpp_cout_get();
Rcpp::Rostream<false>& Rcpp::Rcerr = Rcpp::Rcpp_cerr_get();
#endif

// dentist_rcpp
List dentist_rcpp(const arma::mat& LDmat, uint nSample, const arma::vec& zScore, double pValueThreshold, float propSVD, bool gcControl, int nIter, double gPvalueThreshold, int ncpus, int seed);
RcppExport SEXP _pecotmr_dentist_rcpp(SEXP LDmatSEXP, SEXP nSampleSEXP, SEXP zScoreSEXP, SEXP pValueThresholdSEXP, SEXP propSVDSEXP, SEXP gcControlSEXP, SEXP nIterSEXP, SEXP gPvalueThresholdSEXP, SEXP ncpusSEXP, SEXP seedSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const arma::mat& >::type LDmat(LDmatSEXP);
    Rcpp::traits::input_parameter< uint >::type nSample(nSampleSEXP);
    Rcpp::traits::input_parameter< const arma::vec& >::type zScore(zScoreSEXP);
    Rcpp::traits::input_parameter< double >::type pValueThreshold(pValueThresholdSEXP);
    Rcpp::traits::input_parameter< float >::type propSVD(propSVDSEXP);
    Rcpp::traits::input_parameter< bool >::type gcControl(gcControlSEXP);
    Rcpp::traits::input_parameter< int >::type nIter(nIterSEXP);
    Rcpp::traits::input_parameter< double >::type gPvalueThreshold(gPvalueThresholdSEXP);
    Rcpp::traits::input_parameter< int >::type ncpus(ncpusSEXP);
    Rcpp::traits::input_parameter< int >::type seed(seedSEXP);
    rcpp_result_gen = Rcpp::wrap(dentist_rcpp(LDmat, nSample, zScore, pValueThreshold, propSVD, gcControl, nIter, gPvalueThreshold, ncpus, seed));
    return rcpp_result_gen;
END_RCPP
}
// dentist_rcpp
List dentist_rcpp(const arma::mat& LDmat, uint nSample, const arma::vec& zScore, double pValueThreshold, float propSVD, bool gcControl, int nIter, double gPvalueThreshold, int ncpus, int seed);
RcppExport SEXP _pecotmr_dentist_rcpp(SEXP LDmatSEXP, SEXP nSampleSEXP, SEXP zScoreSEXP, SEXP pValueThresholdSEXP, SEXP propSVDSEXP, SEXP gcControlSEXP, SEXP nIterSEXP, SEXP gPvalueThresholdSEXP, SEXP ncpusSEXP, SEXP seedSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const arma::mat& >::type LDmat(LDmatSEXP);
    Rcpp::traits::input_parameter< uint >::type nSample(nSampleSEXP);
    Rcpp::traits::input_parameter< const arma::vec& >::type zScore(zScoreSEXP);
    Rcpp::traits::input_parameter< double >::type pValueThreshold(pValueThresholdSEXP);
    Rcpp::traits::input_parameter< float >::type propSVD(propSVDSEXP);
    Rcpp::traits::input_parameter< bool >::type gcControl(gcControlSEXP);
    Rcpp::traits::input_parameter< int >::type nIter(nIterSEXP);
    Rcpp::traits::input_parameter< double >::type gPvalueThreshold(gPvalueThresholdSEXP);
    Rcpp::traits::input_parameter< int >::type ncpus(ncpusSEXP);
    Rcpp::traits::input_parameter< int >::type seed(seedSEXP);
    rcpp_result_gen = Rcpp::wrap(dentist_rcpp(LDmat, nSample, zScore, pValueThreshold, propSVD, gcControl, nIter, gPvalueThreshold, ncpus, seed));
    return rcpp_result_gen;
END_RCPP
}
// rcpp_mr_ash_rss
List rcpp_mr_ash_rss(const NumericVector& bhat, const NumericVector& shat, const NumericVector& z, const NumericMatrix& R, double var_y, int n, double sigma2_e, const NumericVector& s0, const NumericVector& w0, const NumericVector& mu1_init, double tol, int max_iter, bool update_w0, bool update_sigma, bool compute_ELBO, bool standardize, int ncpus);
RcppExport SEXP _pecotmr_rcpp_mr_ash_rss(SEXP bhatSEXP, SEXP shatSEXP, SEXP zSEXP, SEXP RSEXP, SEXP var_ySEXP, SEXP nSEXP, SEXP sigma2_eSEXP, SEXP s0SEXP, SEXP w0SEXP, SEXP mu1_initSEXP, SEXP tolSEXP, SEXP max_iterSEXP, SEXP update_w0SEXP, SEXP update_sigmaSEXP, SEXP compute_ELBOSEXP, SEXP standardizeSEXP, SEXP ncpusSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const NumericVector& >::type bhat(bhatSEXP);
    Rcpp::traits::input_parameter< const NumericVector& >::type shat(shatSEXP);
    Rcpp::traits::input_parameter< const NumericVector& >::type z(zSEXP);
    Rcpp::traits::input_parameter< const NumericMatrix& >::type R(RSEXP);
    Rcpp::traits::input_parameter< double >::type var_y(var_ySEXP);
    Rcpp::traits::input_parameter< int >::type n(nSEXP);
    Rcpp::traits::input_parameter< double >::type sigma2_e(sigma2_eSEXP);
    Rcpp::traits::input_parameter< const NumericVector& >::type s0(s0SEXP);
    Rcpp::traits::input_parameter< const NumericVector& >::type w0(w0SEXP);
    Rcpp::traits::input_parameter< const NumericVector& >::type mu1_init(mu1_initSEXP);
    Rcpp::traits::input_parameter< double >::type tol(tolSEXP);
    Rcpp::traits::input_parameter< int >::type max_iter(max_iterSEXP);
    Rcpp::traits::input_parameter< bool >::type update_w0(update_w0SEXP);
    Rcpp::traits::input_parameter< bool >::type update_sigma(update_sigmaSEXP);
    Rcpp::traits::input_parameter< bool >::type compute_ELBO(compute_ELBOSEXP);
    Rcpp::traits::input_parameter< bool >::type standardize(standardizeSEXP);
    Rcpp::traits::input_parameter< int >::type ncpus(ncpusSEXP);
    rcpp_result_gen = Rcpp::wrap(rcpp_mr_ash_rss(bhat, shat, z, R, var_y, n, sigma2_e, s0, w0, mu1_init, tol, max_iter, update_w0, update_sigma, compute_ELBO, standardize, ncpus));
    return rcpp_result_gen;
END_RCPP
}
// prs_cs_rcpp
Rcpp::List prs_cs_rcpp(double a, double b, Rcpp::Nullable<double> phi, Rcpp::NumericVector bhat, Rcpp::Nullable<Rcpp::NumericVector> maf, int n, Rcpp::List ld_blk, int n_iter, int n_burnin, int thin, bool verbose, Rcpp::Nullable<unsigned int> seed);
RcppExport SEXP _pecotmr_prs_cs_rcpp(SEXP aSEXP, SEXP bSEXP, SEXP phiSEXP, SEXP bhatSEXP, SEXP mafSEXP, SEXP nSEXP, SEXP ld_blkSEXP, SEXP n_iterSEXP, SEXP n_burninSEXP, SEXP thinSEXP, SEXP verboseSEXP, SEXP seedSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< double >::type a(aSEXP);
    Rcpp::traits::input_parameter< double >::type b(bSEXP);
    Rcpp::traits::input_parameter< Rcpp::Nullable<double> >::type phi(phiSEXP);
    Rcpp::traits::input_parameter< Rcpp::NumericVector >::type bhat(bhatSEXP);
    Rcpp::traits::input_parameter< Rcpp::Nullable<Rcpp::NumericVector> >::type maf(mafSEXP);
    Rcpp::traits::input_parameter< int >::type n(nSEXP);
    Rcpp::traits::input_parameter< Rcpp::List >::type ld_blk(ld_blkSEXP);
    Rcpp::traits::input_parameter< int >::type n_iter(n_iterSEXP);
    Rcpp::traits::input_parameter< int >::type n_burnin(n_burninSEXP);
    Rcpp::traits::input_parameter< int >::type thin(thinSEXP);
    Rcpp::traits::input_parameter< bool >::type verbose(verboseSEXP);
    Rcpp::traits::input_parameter< Rcpp::Nullable<unsigned int> >::type seed(seedSEXP);
    rcpp_result_gen = Rcpp::wrap(prs_cs_rcpp(a, b, phi, bhat, maf, n, ld_blk, n_iter, n_burnin, thin, verbose, seed));
    return rcpp_result_gen;
END_RCPP
}
// qtl_enrichment_rcpp
Rcpp::List qtl_enrichment_rcpp(SEXP r_gwas_pip, SEXP r_qtl_susie_fit, double pi_gwas, double pi_qtl, int ImpN, double shrinkage_lambda, int num_threads);
RcppExport SEXP _pecotmr_qtl_enrichment_rcpp(SEXP r_gwas_pipSEXP, SEXP r_qtl_susie_fitSEXP, SEXP pi_gwasSEXP, SEXP pi_qtlSEXP, SEXP ImpNSEXP, SEXP shrinkage_lambdaSEXP, SEXP num_threadsSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< SEXP >::type r_gwas_pip(r_gwas_pipSEXP);
    Rcpp::traits::input_parameter< SEXP >::type r_qtl_susie_fit(r_qtl_susie_fitSEXP);
    Rcpp::traits::input_parameter< double >::type pi_gwas(pi_gwasSEXP);
    Rcpp::traits::input_parameter< double >::type pi_qtl(pi_qtlSEXP);
    Rcpp::traits::input_parameter< int >::type ImpN(ImpNSEXP);
    Rcpp::traits::input_parameter< double >::type shrinkage_lambda(shrinkage_lambdaSEXP);
    Rcpp::traits::input_parameter< int >::type num_threads(num_threadsSEXP);
    rcpp_result_gen = Rcpp::wrap(qtl_enrichment_rcpp(r_gwas_pip, r_qtl_susie_fit, pi_gwas, pi_qtl, ImpN, shrinkage_lambda, num_threads));
    return rcpp_result_gen;
END_RCPP
}
// sdpr_rcpp
Rcpp::List sdpr_rcpp(const std::vector<double>& bhat, const Rcpp::List& LD, int n, Rcpp::Nullable<Rcpp::NumericVector> per_variant_sample_size, Rcpp::Nullable<Rcpp::IntegerVector> array, double a, double c, size_t M, double a0k, double b0k, int iter, int burn, int thin, unsigned n_threads, int opt_llk, bool verbose);
RcppExport SEXP _pecotmr_sdpr_rcpp(SEXP bhatSEXP, SEXP LDSEXP, SEXP nSEXP, SEXP per_variant_sample_sizeSEXP, SEXP arraySEXP, SEXP aSEXP, SEXP cSEXP, SEXP MSEXP, SEXP a0kSEXP, SEXP b0kSEXP, SEXP iterSEXP, SEXP burnSEXP, SEXP thinSEXP, SEXP n_threadsSEXP, SEXP opt_llkSEXP, SEXP verboseSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const std::vector<double>& >::type bhat(bhatSEXP);
    Rcpp::traits::input_parameter< const Rcpp::List& >::type LD(LDSEXP);
    Rcpp::traits::input_parameter< int >::type n(nSEXP);
    Rcpp::traits::input_parameter< Rcpp::Nullable<Rcpp::NumericVector> >::type per_variant_sample_size(per_variant_sample_sizeSEXP);
    Rcpp::traits::input_parameter< Rcpp::Nullable<Rcpp::IntegerVector> >::type array(arraySEXP);
    Rcpp::traits::input_parameter< double >::type a(aSEXP);
    Rcpp::traits::input_parameter< double >::type c(cSEXP);
    Rcpp::traits::input_parameter< size_t >::type M(MSEXP);
    Rcpp::traits::input_parameter< double >::type a0k(a0kSEXP);
    Rcpp::traits::input_parameter< double >::type b0k(b0kSEXP);
    Rcpp::traits::input_parameter< int >::type iter(iterSEXP);
    Rcpp::traits::input_parameter< int >::type burn(burnSEXP);
    Rcpp::traits::input_parameter< int >::type thin(thinSEXP);
    Rcpp::traits::input_parameter< unsigned >::type n_threads(n_threadsSEXP);
    Rcpp::traits::input_parameter< int >::type opt_llk(opt_llkSEXP);
    Rcpp::traits::input_parameter< bool >::type verbose(verboseSEXP);
    rcpp_result_gen = Rcpp::wrap(sdpr_rcpp(bhat, LD, n, per_variant_sample_size, array, a, c, M, a0k, b0k, iter, burn, thin, n_threads, opt_llk, verbose));
    return rcpp_result_gen;
END_RCPP
}

static const R_CallMethodDef CallEntries[] = {
    {"_pecotmr_dentist_rcpp", (DL_FUNC) &_pecotmr_dentist_rcpp, 10},
    {"_pecotmr_dentist_rcpp", (DL_FUNC) &_pecotmr_dentist_rcpp, 10},
    {"_pecotmr_rcpp_mr_ash_rss", (DL_FUNC) &_pecotmr_rcpp_mr_ash_rss, 17},
    {"_pecotmr_prs_cs_rcpp", (DL_FUNC) &_pecotmr_prs_cs_rcpp, 12},
    {"_pecotmr_qtl_enrichment_rcpp", (DL_FUNC) &_pecotmr_qtl_enrichment_rcpp, 7},
    {"_pecotmr_sdpr_rcpp", (DL_FUNC) &_pecotmr_sdpr_rcpp, 16},
    {NULL, NULL, 0}
};

RcppExport void R_init_pecotmr(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
