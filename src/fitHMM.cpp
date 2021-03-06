#include <Rcpp.h>
#include "scalehmm.h"
#ifdef _OPENMP
#include <omp.h>
#endif

using namespace Rcpp;

static ScaleHMM * hmm; // declare as static outside the function because we only need one and this enables memory-cleanup on R_CheckUserInterrupt()

// [[Rcpp::export]]
List fitHMM(const IntegerVector & counts, const NumericVector & distances, const List & params, const int & algorithm) {

    // access variables by name
    const NumericVector startProbs_initial = as<NumericVector>(params["startProbs_initial"]);
    const NumericMatrix transProbs_initial = as<NumericMatrix>(params["transProbs_initial"]);
		const double transDist = as<double>(params["transDist"]);
		const DataFrame emissionParams_initial = as<DataFrame>(params["emissionParams_initial"]);
    const double eps = as<double>(params["eps"]);
    const double maxtime = as<double>(params["maxtime"]);
    const double maxiter = as<double>(params["maxiter"]);
		const int verbosity = as<int>(params["verbosity"]);

		// Parallelization settings
		#ifdef _OPENMP
		const int num_threads = as<int>(params["numThreads"]);
		omp_set_num_threads(num_threads);
		#endif

		// Initialize the HMM
		hmm = new ScaleHMM(counts, distances, startProbs_initial, transProbs_initial, transDist, emissionParams_initial, verbosity);

		// Estimate parameters
		List results;
		std::string error = "";
		if (algorithm == 1)
		{
			try {
					results = hmm->baumWelch(eps, maxiter, maxtime);
			} catch (std::exception & e) {
					if (verbosity>=1) Rprintf("HMM: Error in Baum-Welch: %s\n", e.what());
					error = e.what();
			}
		}
		else if (algorithm == 2)
		{
			try {
					results = hmm->forward_backward(eps, maxiter, maxtime);
			} catch (std::exception & e) {
					if (verbosity>=1) Rprintf("HMM: Error in Viterbi: %s\n", e.what());
					error = e.what();
			}
		}
		results.push_back(error, "error");

		// Delete the HMM
		delete hmm;
		hmm = NULL; // assign NULL to defuse the additional delete in on.exit() call

		// Return
		return results;

}

// // [[Rcpp::export]]
// List fitBinomialTestHMM(const IntegerVector & counts_total, const IntegerVector & counts_meth, const NumericVector & distances, const List & params, const int & algorithm) {
// 
//     // access variables by name
//     const NumericVector startProbs_initial = as<NumericVector>(params["startProbs_initial"]);
//     const NumericMatrix transProbs_initial = as<NumericMatrix>(params["transProbs_initial"]);
// 		const double transDist = as<double>(params["transDist"]);
// 		const DataFrame emissionParams_initial = as<DataFrame>(params["emissionParams_initial"]);
//     const double eps = as<double>(params["eps"]);
//     const double maxtime = as<double>(params["maxtime"]);
//     const double maxiter = as<double>(params["maxiter"]);
// 		const int min_obs = as<int>(params["minreads"]);
// 		const int verbosity = as<int>(params["verbosity"]);
// 
// 		// Parallelization settings
// 		#ifdef _OPENMP
// 		const int num_threads = as<int>(params["numThreads"]);
// 		omp_set_num_threads(num_threads);
// 		#endif
// 
// 		// Initialize the HMM
// 		hmm = new ScaleHMM(counts_total, counts_meth, distances, startProbs_initial, transProbs_initial, transDist, emissionParams_initial, min_obs, verbosity);
// 
// 		// Estimate parameters
// 		List results;
// 		std::string error = "";
// 		if (algorithm == 1)
// 		{
// 			try {
// 					results = hmm->baumWelch(eps, maxiter, maxtime);
// 			} catch (std::exception & e) {
// 					if (verbosity>=1) Rprintf("HMM: Error in Baum-Welch: %s\n", e.what());
// 					error = e.what();
// 			}
// 		}
// 		else if (algorithm == 2)
// 		{
// 			try {
// 					results = hmm->forward_backward(eps, maxiter, maxtime);
// 			} catch (std::exception & e) {
// 					if (verbosity>=1) Rprintf("HMM: Error in Viterbi: %s\n", e.what());
// 					error = e.what();
// 			}
// 		}
// 		results.push_back(error, "error");
// 
// 		// Delete the HMM
// 		delete hmm;
// 		hmm = NULL; // assign NULL to defuse the additional delete in on.exit() call
// 
// 		// Return
// 		return results;
// 
// }
// 
// // [[Rcpp::export]]
// List fitBinomialTestHMMcontext(const IntegerVector & counts_total, const IntegerVector & counts_meth, const IntegerVector & context, const NumericVector & distances, const List & params, const int & algorithm) {
// 
//     // access variables by name
//     const NumericVector startProbs_initial = as<NumericVector>(params["startProbs_initial"]);
//     const NumericMatrix transProbs_initial = as<NumericMatrix>(params["transProbs_initial"]);
// 		const double transDist = as<double>(params["transDist"]);
// 		const List emissionParams_initial = as<List>(params["emissionParams_initial"]);
//     const double eps = as<double>(params["eps"]);
//     const double maxtime = as<double>(params["maxtime"]);
//     const double maxiter = as<double>(params["maxiter"]);
// 		const int min_obs = as<int>(params["minreads"]);
// 		const int verbosity = as<int>(params["verbosity"]);
// 
// 		// Parallelization settings
// 		#ifdef _OPENMP
// 		const int num_threads = as<int>(params["numThreads"]);
// 		omp_set_num_threads(num_threads);
// 		#endif
// 
// 		// Initialize the HMM
// 		hmm = new ScaleHMM(counts_total, counts_meth, context, distances, startProbs_initial, transProbs_initial, transDist, emissionParams_initial, min_obs, verbosity);
// 
// 		// Estimate parameters
// 		List results;
// 		std::string error = "";
// 		if (algorithm == 1)
// 		{
// 			try {
// 					results = hmm->baumWelch(eps, maxiter, maxtime);
// 			} catch (std::exception & e) {
// 					if (verbosity>=1) Rprintf("HMM: Error in Baum-Welch: %s\n", e.what());
// 					error = e.what();
// 			}
// 		}
// 		else if (algorithm == 2)
// 		{
// 			try {
// 					results = hmm->forward_backward(eps, maxiter, maxtime);
// 			} catch (std::exception & e) {
// 					if (verbosity>=1) Rprintf("HMM: Error in Viterbi: %s\n", e.what());
// 					error = e.what();
// 			}
// 		}
// 		results.push_back(error, "error");
// 
// 		// Delete the HMM
// 		delete hmm;
// 		hmm = NULL; // assign NULL to defuse the additional delete in on.exit() call
// 
// 		// Return
// 		return results;
// 
// }
// 
// // [[Rcpp::export]]
// List fitHMMratio(const NumericVector & ratio, const NumericVector & distances, const List & params, const int & algorithm) {
// 
//     // access variables by name
//     const NumericVector startProbs_initial = as<NumericVector>(params["startProbs_initial"]);
//     const NumericMatrix transProbs_initial = as<NumericMatrix>(params["transProbs_initial"]);
// 		const double transDist = as<double>(params["transDist"]);
// 		const DataFrame emissionParams_initial = as<DataFrame>(params["emissionParams_initial"]);
//     const double eps = as<double>(params["eps"]);
//     const double maxtime = as<double>(params["maxtime"]);
//     const double maxiter = as<double>(params["maxiter"]);
// 		const int verbosity = as<int>(params["verbosity"]);
// 
// 		// Parallelization settings
// 		#ifdef _OPENMP
// 		const int num_threads = as<int>(params["numThreads"]);
// 		omp_set_num_threads(num_threads);
// 		#endif
// 
// 		// Initialize the HMM
// 		hmm = new ScaleHMM(ratio, distances, startProbs_initial, transProbs_initial, transDist, emissionParams_initial, verbosity);
// 
// 		// Estimate parameters
// 		List results;
// 		std::string error = "";
// 		if (algorithm == 1)
// 		{
// 			try {
// 					results = hmm->baumWelch(eps, maxiter, maxtime);
// 			} catch (std::exception & e) {
// 					if (verbosity>=1) Rprintf("HMM: Error in Baum-Welch: %s\n", e.what());
// 					error = e.what();
// 			}
// 		}
// 		else if (algorithm == 2)
// 		{
// 			try {
// 					results = hmm->forward_backward(eps, maxiter, maxtime);
// 			} catch (std::exception & e) {
// 					if (verbosity>=1) Rprintf("HMM: Error in Viterbi: %s\n", e.what());
// 					error = e.what();
// 			}
// 		}
// 		results.push_back(error, "error");
// 
// 		// Delete the HMM
// 		delete hmm;
// 		hmm = NULL; // assign NULL to defuse the additional delete in on.exit() call
// 
// 		// Return
// 		return results;
// 
// }
// 
// 
// // [[Rcpp::export]]
// List fitMultiHMM(const IntegerMatrix & counts, const NumericVector & distances, const List & params) {
// 
//     // access variables by name
//     const NumericVector startProbs_initial = as<NumericVector>(params["startProbs_initial"]);
//     const NumericMatrix transProbs_initial = as<NumericMatrix>(params["transProbs_initial"]);
// 		const double transDist = as<double>(params["transDist"]);
// 		const List emissionParamsList = as<List>(params["emissionParamsList"]);
//     const double eps = as<double>(params["eps"]);
//     const double maxtime = as<double>(params["maxtime"]);
//     const double maxiter = as<double>(params["maxiter"]);
// 		const int verbosity = as<int>(params["verbosity"]);
// 		const NumericVector determinant = as<NumericVector>(params["determinant"]);
// 		const List cor_mat_inv = as<List>(params["correlationMatrixInverse"]);
// 		const DataFrame statedef = as<DataFrame>(params["statedef"]);
// 
// 		// Parallelization settings
// 		#ifdef _OPENMP
// 		const int num_threads = as<int>(params["numThreads"]);
// 		omp_set_num_threads(num_threads);
// 		#endif
// 
// 		// Initialize the HMM
// 		hmm = new ScaleHMM(counts, distances, startProbs_initial, transProbs_initial, transDist, emissionParamsList, verbosity, cor_mat_inv, determinant, statedef);
// 
// 		// Estimate parameters
// 		List results;
// 		std::string error = "";
// 		try {
// 				results = hmm->baumWelch(eps, maxiter, maxtime);
// 		} catch (std::exception & e) {
// 				if (verbosity>=1) Rprintf("HMM: Error in Baum-Welch: %s\n", e.what());
// 				error = e.what();
// 		}
// 		results.push_back(error, "error");
// 
// 		// Delete the HMM
// 		delete hmm;
// 		hmm = NULL; // assign NULL to defuse the additional delete in on.exit() call
// 
// 		// Return
// 		return results;
// 
// }

