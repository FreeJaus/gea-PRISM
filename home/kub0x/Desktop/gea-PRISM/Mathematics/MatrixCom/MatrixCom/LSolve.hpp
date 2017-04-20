#pragma once

#include <vector>
#include <lapacke/lapacke.h>
#include "Singleton.hpp"
#include "LEquation.hpp"

class LSolve : public Singleton<LSolve> {


	friend class Singleton<LSolve>;

	private:

		LSolve() {}

		LEquation* GetEquation(std::vector<double> a, int row, int col, int rowsize);
		float* CoefMatrix(std::vector<double> a);
		void GetRank(lapack_int n, lapack_int nrhs, float *a, lapack_int lda, lapack_int ldb, lapack_int *r);
		void SolveSystem(lapack_int n, lapack_int nrhs, double *a, double *b, lapack_int lda, lapack_int ldb);
		std::vector<double*> UnDetCoef(float *s, std::vector<double> vars, lapack_int rank);
		void PrintMatrix(double *m, int r, int c);
		
	public:

		double* CreateSystem(std::vector<double> a, std::vector<double> vars);
		void MulMatrix(lapack_int n, std::vector<double> v, double* rhs, std::vector<double> vars, lapack_int lda, lapack_int ldb);

};