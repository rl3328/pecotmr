// This function implements, to our knowledge, the methods decribed in the DENTIST paper
// https://github.com/Yves-CHEN/DENTIST/tree/master#Citations
// Some codes are adapted and rewritten from https://github.com/Yves-CHEN/DENTIST/tree/master
// to fit the Rcpp implementation.
// The code reflects our understanding and interpretation of DENTIST method which may difer in details
// from the author's original proposal, although in various tests we find that our implementation and the
// original results are mostly identical
#include <RcppArmadillo.h>
#include <omp.h> // Required for parallel processing
#include <algorithm>
#include <random>
#include <vector>
#include <numeric> // For std::iota
#include <gsl/gsl_cdf.h>
#include <unordered_set>

// Enable C++11 via this plugin (Rcpp 0.10.3 or later)
// [[Rcpp::depends(RcppArmadillo)]]
// [[Rcpp::plugins(cpp11)]]
// [[Rcpp::plugins(openmp)]]

using namespace Rcpp;
using namespace arma;

// Assuming sort_indexes is defined as provided
std::vector<size_t> sort_indexes(const std::vector<int>& v, unsigned int theSize) {
	std::vector<size_t> idx(theSize);
	std::iota(idx.begin(), idx.end(), 0);
	std::sort(idx.begin(), idx.end(), [&v](size_t i1, size_t i2) {
		return v[i1] < v[i2];
	});
	return idx;
}

// Improved generateSetOfNumbers function using C++11 random and std::unordered_set
std::vector<size_t> generateSetOfNumbers(int SIZE, int seed) {
	std::vector<int> numbers(SIZE);
	std::unordered_set<int> uniqueNumbers;
	std::mt19937 rng(seed);
	std::uniform_int_distribution<int> dist(0, INT_MAX);

	for (int i = 0; i < SIZE; ++i) {
		int tempNum;
		do {
			tempNum = dist(rng);
		} while (!uniqueNumbers.insert(tempNum).second);
		numbers[i] = tempNum;
	}

	return sort_indexes(numbers, SIZE);
}

// Get a quantile value
double getQuantile(const std::vector<double>& dat, double whichQuantile) {
	std::vector<double> sortedData = dat;
	std::sort(sortedData.begin(), sortedData.end());
	size_t pos = ceil(sortedData.size() * whichQuantile) - 1;
	return sortedData.at(pos);
}

// Get a quantile value based on grouping
double getQuantile2(const std::vector<double>& dat, const std::vector<uint>& grouping, double whichQuantile, bool invert_grouping = false) {
	std::vector<double> filteredData;
	for (size_t i = 0; i < dat.size(); ++i) {
		// Apply grouping filter with optional inversion
		if ((grouping[i] == 1) != invert_grouping) {
			filteredData.push_back(dat[i]);
		}
	}
	if (filteredData.size() < 50) return 0;
	return getQuantile(filteredData, whichQuantile);
}

// Get a quantile value based on grouping
double getQuantile2_chen_et_al(const std::vector<double> &dat, std::vector<uint> grouping, double whichQuantile)
{
	uint sum = std::accumulate(grouping.begin(), grouping.end(), 0);

	if (sum < 50)
	{
		return 0;
	}

	std::vector<double> diff2;
	for (uint i = 0; i < dat.size(); i++)
	{
		if (grouping[i] == 1)
			diff2.push_back(dat[i]);
	}
	return getQuantile(diff2, whichQuantile);
}

// Calculate minus log p-value of chi-squared statistic
double minusLogPvalueChisq2(double stat) {
	double p = 1.0 - gsl_cdf_chisq_P(stat, 1.0);
	return -log10(p);
}

// Perform one iteration of the algorithm, assuming LD_mat is an arma::mat
void oneIteration(const arma::mat& LD_mat, const std::vector<uint>& idx, const std::vector<uint>& idx2,
                  const arma::vec& zScore, arma::vec& imputedZ, arma::vec& rsqList, arma::vec& zScore_e,
                  uint nSample, float probSVD, int ncpus) {
	int nProcessors = omp_get_max_threads();
	if (ncpus < nProcessors) nProcessors = ncpus;
	omp_set_num_threads(nProcessors);

	uint K = std::min(static_cast<uint>(idx.size()), nSample) * probSVD;

	arma::vec zScore_eigen(idx.size());
	arma::mat LD_it(idx2.size(), idx.size());
	arma::mat VV(idx.size(), idx.size());

	// Fill LD_it and VV matrices using direct indexing
    #pragma omp parallel for collapse(2)
	for (size_t i = 0; i < idx2.size(); i++) {
		for (size_t k = 0; k < idx.size(); k++) {
			LD_it(i, k) = LD_mat.at(idx2[i] * LD_mat.n_cols + idx[k]);
		}
	}

    #pragma omp parallel for
	for (size_t i = 0; i < idx.size(); i++) {
		zScore_eigen(i) = zScore[idx[i]];
		for (size_t j = 0; j < idx.size(); j++) {
			VV(i, j) = LD_mat.at(idx[i] * LD_mat.n_rows + idx[j]);
		}
	}

	// Eigen decomposition
	arma::vec eigval;
	arma::mat eigvec;
	arma::eig_sym(eigval, eigvec, VV);

	int nRank = eigvec.n_rows;
	int nZeros = arma::sum(eigval < 0.0001);
	nRank -= nZeros;
	K = std::min(K, static_cast<uint>(nRank));
	if (K <= 1) {
		Rcpp::stop("Rank of eigen matrix <= 1");
	}
	arma::mat ui = arma::eye<arma::mat>(eigvec.n_rows, K);
	arma::mat wi = arma::eye<arma::mat>(K, K);
	for (uint m = 0; m < K; ++m) {
		int j = eigvec.n_rows - m - 1;
		ui.col(m) = eigvec.col(j);
		wi(m, m) = 1.0 / eigval(j);
	}

	// Calculate imputed Z scores and R squared values
	arma::mat beta = LD_it * ui * wi;
	arma::vec zScore_eigen_imp = beta * (ui.t() * zScore_eigen);
	arma::mat product = beta * (ui.t() * LD_it.t());
	arma::vec rsq_eigen = product.diag();

    #pragma omp parallel for
	for (size_t i = 0; i < idx2.size(); ++i) {
		imputedZ[idx2[i]] = zScore_eigen_imp(i);
		rsqList[idx2[i]] = rsq_eigen(i);
		rsqList[idx2[i]] = std::min(rsq_eigen(i), 1.0); // Ensure rsq does not exceed 1
		if (rsq_eigen(i) >= 1) {
			// Handle the case where rsq_eigen is unexpectedly high
			Rcpp::warning("Adjusted rsq_eigen value exceeding 1: " + std::to_string(rsq_eigen(i)));
		}
		uint j = idx2[i];
		zScore_e[j] = (zScore[j] - imputedZ[j]) / std::sqrt(LD_mat(j, j) - rsqList[j]);
	}
}

/**
 * @brief Executes DENTIST algorithm for quality control in GWAS summary data: the iterative imputation function.
 *
 * DENTIST (Detecting Errors iN analyses of summary staTISTics) identifies and removes problematic variants
 * in GWAS summary data by comparing observed GWAS statistics to predicted values based on linkage disequilibrium (LD)
 * information from a reference panel. It helps detect genotyping/imputation errors, allelic errors, and heterogeneity
 * between GWAS and LD reference samples, improving the reliability of subsequent analyses.
 *
 * @param LD_mat The linkage disequilibrium (LD) matrix from a reference panel, as an arma::mat object.
 * @param nSample The sample size used in the GWAS whose summary statistics are being analyzed.
 * @param zScore A vector of Z-scores from GWAS summary statistics.
 * @param pValueThreshold Threshold for the p-value below which variants are considered for quality control.
 * @param propSVD Proportion of singular value decomposition (SVD) components retained in the analysis.
 * @param gcControl A boolean flag to apply genetic control corrections.
 * @param nIter The number of iterations to run the DENTIST algorithm.
 * @param gPvalueThreshold P-value threshold for grouping variants into significant and null categories.
 * @param ncpus The number of CPU cores to use for parallel processing.
 * @param seed Seed for random number generation, affecting the selection of variants for analysis.
 *
 * @return A List object containing:
 * - imputed_z: A vector of imputed Z-scores for each marker.
 * - rsq: A vector of R-squared values for each marker, indicating goodness of fit.
 * - corrected_z: A vector of adjusted Z-scores after error detection.
 * - iter_to_correct: An integer vector indicating the iteration in which each marker passed the quality control.
 * - is_problematic: A binary vector indicating whether each marker is considered problematic (1) or not (0).
 *
 * @note The function is designed for use in Rcpp and requires Armadillo for matrix operations and OpenMP for parallel processing.
 */

// [[Rcpp::export]]
List dentist_iterative_impute(const arma::mat& LD_mat, uint nSample, const arma::vec& zScore,
                              double pValueThreshold, float propSVD, bool gcControl, int nIter,
                              double gPvalueThreshold, int ncpus, int seed, bool correct_chen_et_al_bug) {
	// Set number of threads for parallel processing
	int nProcessors = omp_get_max_threads();
	if (ncpus < nProcessors) nProcessors = ncpus;
	omp_set_num_threads(nProcessors);

	uint markerSize = zScore.size();
	// Initialization based on the seed input
	std::vector<size_t> randOrder = generateSetOfNumbers(markerSize, seed);
	std::vector<uint> idx, idx2;
	idx.reserve(markerSize / 2);
	idx2.reserve(markerSize / 2);
	std::vector<uint> fullIdx(randOrder.begin(), randOrder.end());

	// Determining indices for partitioning
	for (uint i = 0; i < markerSize; ++i) {
		if (randOrder[i] > markerSize / 2) idx.push_back(i);
		else idx2.push_back(i);
	}

	std::vector<uint> groupingGWAS(markerSize, 0);
	for (uint i = 0; i < markerSize; ++i) {
		if (minusLogPvalueChisq2(std::pow(zScore(i), 2)) > -log10(gPvalueThreshold)) {
			groupingGWAS[i] = 1;
		}
	}

	arma::vec imputedZ = arma::zeros<arma::vec>(markerSize);
	arma::vec rsq = arma::zeros<arma::vec>(markerSize);
	arma::vec zScore_e = arma::zeros<arma::vec>(markerSize);
	arma::ivec iterID = arma::zeros<arma::ivec>(markerSize);

	std::vector<double> diff(idx2.size());
	std::vector<uint> grouping_tmp(idx2.size());

	for (int t = 0; t < nIter; ++t) {
		std::vector<uint> idx2_QCed;

		// Perform iteration with current subsets
		oneIteration(LD_mat, idx, idx2, zScore, imputedZ, rsq, zScore_e, nSample, propSVD, ncpus);

		// Assess differences and grouping for thresholding
		for (size_t i = 0; i < idx2.size(); ++i) {
			diff[i] = std::abs(zScore_e[idx2[i]]);
			grouping_tmp[i] = groupingGWAS[idx2[i]];
		}

		double threshold = getQuantile(diff, 0.995);
		double threshold1, threshold0;
		/*
		        In the original DENTIST method, whenever you call !grouping_tmp, it is going to change the original value of grouping_tmp as well.
		        For example, if grouping_tmp is (0,0,1,1,1), and you run:
		        double threshold0 = getQuantile2 <double> (diff,!grouping_tmp , (99.5/100.0)) ;
		        then your grouping_tmp will become (1,1,0,0,0) even you are just calling it in the function.
		        https://github.com/Yves-CHEN/DENTIST/blob/2fefddb1bbee19896a30bf56229603561ea1dba8/main/inversion.cpp#L647
		        https://github.com/Yves-CHEN/DENTIST/blob/2fefddb1bbee19896a30bf56229603561ea1dba8/main/inversion.cpp#L675
		        Thus if we correct the original DENTIST code, i.e., correct_chen_et_al_bug = TRUE,
		                we go through our function, getQuantile2, which doesn't have this issue
		                else, i.e., correct_chen_et_al_bug = TRUE, it goes through the original function getQuantile2_chen_et_al
		 */
		if (correct_chen_et_al_bug) {
			threshold1 = getQuantile2(diff, grouping_tmp, 0.995, false);
			threshold0 = getQuantile2(diff, grouping_tmp, 0.995, true);
		} else {
			threshold1 = getQuantile2_chen_et_al(diff, grouping_tmp, 0.995);
			std::transform(grouping_tmp.begin(), grouping_tmp.end(), grouping_tmp.begin(), [](uint val) {
				return 1 - val;
			});
			threshold0 = getQuantile2_chen_et_al(diff, grouping_tmp, 0.995);
		}

		if (threshold1 == 0) {
			threshold1 = threshold;
			threshold0 = threshold;
		}
		if (correct_chen_et_al_bug || nIter - 2 >= 0) {
			/*
			   In the original DENTIST method, if t=0 (first iteration) and nIter is 1,
			   t is defined as a uint (unassigned integer)
			   https://github.com/Yves-CHEN/DENTIST/blob/2fefddb1bbee19896a30bf56229603561ea1dba8/main/inversion.cpp#L628
			   and it will treat t (which is 0) no larger than nIter-2 (which is -1) which is wrong
			   Thus if we correct the original DENTIST code, i.e., correct_chen_et_al_bug = TRUE, or when nIter - 2 >=0,
			   it will compare t and nIter as we expect.
			   and if we want to keep the original DENTIST code, i.e., correct_chen_et_al_bug = FALSE, then it will skip this if condition for t > nIter - 2
			 */
			if (t > nIter - 2) {
				threshold0 = threshold;
				threshold1 = threshold;
			}
		}
		// Apply threshold-based filtering for QC
		for (size_t i = 0; i < diff.size(); ++i) {
			if ((grouping_tmp[i] == 1 && diff[i] <= threshold1) ||
			    (grouping_tmp[i] == 0 && diff[i] <= threshold0)) {
				idx2_QCed.push_back(idx2[i]);
			}
		}

		// Perform another iteration with updated sets of indices (idx and idx2_QCed)
		oneIteration(LD_mat, idx2_QCed, idx, zScore, imputedZ, rsq, zScore_e, nSample, propSVD, ncpus);

		// Recalculate differences and groupings after the iteration
		diff.resize(fullIdx.size());
		grouping_tmp.resize(fullIdx.size());
		for (size_t i = 0; i < fullIdx.size(); ++i) {
			diff[i] = std::abs(zScore_e[fullIdx[i]]);
			grouping_tmp[i] = groupingGWAS[fullIdx[i]];
		}

		// Re-determine thresholds based on the recalculated differences and groupings
		threshold = getQuantile(diff, 0.995);
		if (correct_chen_et_al_bug) {
			// FIXME: explain the story here
			threshold1 = getQuantile2(diff, grouping_tmp, 0.995, false);
			threshold0 = getQuantile2(diff, grouping_tmp, 0.995, true);
		} else {
			threshold1 = getQuantile2_chen_et_al(diff, grouping_tmp, 0.995);
			std::transform(grouping_tmp.begin(), grouping_tmp.end(), grouping_tmp.begin(), [](uint val) {
				return 1 - val;
			});
			threshold0 = getQuantile2_chen_et_al(diff, grouping_tmp, 0.995);
		}
		if (threshold1 == 0) {
			threshold1 = threshold;
			threshold0 = threshold;
		}

		if (correct_chen_et_al_bug || nIter - 2 >= 0) {
			if (t > nIter - 2) {
				threshold0 = threshold;
				threshold1 = threshold;
			}
		}
		// Adjust for genetic control and inflation factor if necessary
		std::vector<double> chisq(fullIdx.size());
		for (size_t i = 0; i < fullIdx.size(); ++i) {
			chisq[i] = std::pow(zScore_e[fullIdx[i]], 2);
		}

		// Calculate the median chi-squared value as the inflation factor
		std::nth_element(chisq.begin(), chisq.begin() + chisq.size() / 2, chisq.end());
		double medianChisq = chisq[chisq.size() / 2];
		double inflationFactor = medianChisq / 0.46;

		std::vector<uint> fullIdx_tmp;
		for (size_t i = 0; i < fullIdx.size(); ++i) {
			double currentDiffSquared = std::pow(diff[i], 2);

			if (gcControl) {
				// When gcControl is true, check if the variant passes the adjusted threshold
				if (!(diff[i] > threshold && minusLogPvalueChisq2(currentDiffSquared / inflationFactor) > -log10(pValueThreshold))) {
					fullIdx_tmp.push_back(fullIdx[i]);
				}
			} else {
				// When gcControl is false, simply check if the variant passes the basic threshold
				if (minusLogPvalueChisq2(currentDiffSquared) < -log10(pValueThreshold)) {
					if ((groupingGWAS[fullIdx[i]] == 1 && diff[i] <= threshold1) ||
					    (groupingGWAS[fullIdx[i]] == 0 && diff[i] <= threshold0)) {
						fullIdx_tmp.push_back(fullIdx[i]);
						iterID[fullIdx[i]]++;
					}
				}
			}
		}

		// Update the indices for the next iteration based on filtering criteria
		fullIdx = fullIdx_tmp;
		randOrder = generateSetOfNumbers(fullIdx.size(), seed + t * seed); // Update seed for randomness
		idx.clear();
		idx2.clear();
		for (size_t i = 0; i < fullIdx.size(); ++i) {
			if (randOrder[i] > fullIdx.size() / 2) idx.push_back(fullIdx[i]);
			else idx2.push_back(fullIdx[i]);
		}
	}

	return List::create(Named("original_z") = zScore,
	                    Named("imputed_z") = imputedZ,
	                    Named("rsq") = rsq,
	                    Named("corrected_z") = zScore_e,
	                    Named("iter_to_correct") = iterID);
}