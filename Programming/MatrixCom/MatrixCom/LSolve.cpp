#include <iostream>
#include <cstring>
#include <cmath>
#include <limits>
#include <iomanip>
#include "LSolve.hpp"

LEquation* LSolve::GetEquation(std::vector<double> a, int row, int col, int rowsize){
	std::vector<Unknown*> uk;
	int round = 0;
	for (int i = rowsize*row; i < rowsize*row + rowsize; i++){
		uk.push_back(new Unknown(a.at(i), round*rowsize + col));
		round++;
	}
	round = 0;
	for (int i = col ; i <= (rowsize-1)*rowsize +col; i= round*rowsize + col){
		uk.push_back(new Unknown(-(a.at(i)), rowsize*row + round));
		round++;
	}
	return new LEquation(uk, rowsize);
}

void LSolve::GetRank(lapack_int n, lapack_int nrhs, float *a, lapack_int lda, lapack_int ldb, lapack_int *r){
	float b[lda*nrhs] = {0};
	lapack_int x[n] = {0};
	if (LAPACKE_sgelsy(LAPACK_ROW_MAJOR, n, lda, nrhs, a, lda, b, ldb, x, 0.007f, r) != 0)
		std::cout << "sgelsy failed!" << std::endl;
}

void LSolve::SolveSystem(lapack_int n, lapack_int nrhs, double *a, double *b, lapack_int lda, lapack_int ldb){
	lapack_int ipiv[n] = {0};
	if (LAPACKE_dgesv(LAPACK_ROW_MAJOR, n, nrhs, a, lda, ipiv, b, ldb) !=0)
		std::cout << "dgesv failed!" << std::endl;
}

float* LSolve::CoefMatrix(std::vector<double> a){
	int rowsize = sqrt(a.size()), r = -1;
	float *m = new float[rowsize*rowsize*rowsize*rowsize];
	for (int i = 0; i < rowsize; i++){
		for (int j = 0; j < rowsize; j++){
			r++;
			LEquation *leq = GetEquation(a, i, j, rowsize);
			leq->Simplify();
			leq->Sort();
			//leq->Print();
			int *cm = leq->CoefMatrix();
			for (int k = 0; k < rowsize*rowsize; k++){
				m[r*rowsize*rowsize + k] = cm[k];
			}
			delete leq;
		}
	}
	return m;
}

double* LSolve::CreateSystem(std::vector<double> a, std::vector<double> vars){
	float *m = CoefMatrix(a);
	lapack_int n=a.size(), nrhs = 1, lda = a.size(), ldb = 1, r = 0;
	GetRank(n, nrhs, m, lda, ldb, &r);
	delete [] m;
	std::cout << "Rank: " << r << std::endl;
	m = CoefMatrix(a);
	std::vector<double*> v = UnDetCoef(m, vars, r);
	double *um = v.at(0);
	double *ub = v.at(1);

	n = r, lda = r, ldb = 1;

	SolveSystem(n, nrhs, um, ub, lda, ldb);

	delete [] m;
	delete [] um;
	return ub;
}

std::vector<double*> LSolve::UnDetCoef(float *s, std::vector<double> vars, int rank){
	//Reduces a system of n*n l.eq to a rank*rank l.eqs
	//N - rank variables
	int size = vars.size() + rank;
	int _size = size;
	std::vector<double*> v;
	double *m = new double[rank*rank];
	double *rhs = new double[rank];
	v.push_back(m); v.push_back(rhs);
	float _rhs[_size] = {0};
	for (int i = 0; i < vars.size(); i++){
		for (int j = 0; j < size; j++){
			_rhs[j] -= s[j*_size + size - 1]*vars.at(i);
		}
		size--;
	}
	for (int i=0; i < rank; i++)
		for (int j=0; j < rank; j++)
			m[i*rank + j] = s[i*_size + j];
	for (int i=0; i < size; i++)
		rhs[i] = _rhs[i];
	return v;

}

extern "C" void dgemm_(const char *TRANSA, const char *TRANSB, const int *M, const int *N, const int *K, double *ALPHA, double *A, const int *LDA, double *B, const int *LDB, double *BETA, double *C, const int *LDC);

void LSolve::MulMatrix(lapack_int n, std::vector<double> v, double* rhs, std::vector<double> vars, lapack_int lda, lapack_int ldb){

	double *a = new double[v.size()];
	double *b = new double[v.size()];
	for (int i = 0; i < v.size(); i++)
		a[i] = v.at(i);
	for (int i = 0; i < v.size() - vars.size(); i++)
		b[i] = rhs[i];
	int k = vars.size()-1;
	for (int i = v.size() - vars.size(); i < v.size(); i++){
		b[i] = vars.at(k);
		k--;
	}
	
	char TRANS = 'T';
	double ALPHA = 1.0d;
	double BETA = 0.0d;
	double C[n*ldb] = {0};

	dgemm_(&TRANS,&TRANS, &n, &lda, &lda, &ALPHA, a, &lda, b, &ldb, &BETA, C, &lda);
	PrintMatrix(C, n, lda);
	memset(C, 0, sizeof(C));

	dgemm_(&TRANS,&TRANS, &n, &lda, &lda, &ALPHA, b, &ldb, a, &lda, &BETA, C, &lda);
	PrintMatrix(C, n, lda);
}

void LSolve::PrintMatrix(double *m, int r, int c){
	for (int i = 0; i < r; i++){
		for (int j = 0; j < c; j++){
			std::cout << std::setprecision (15) << m[i*c +j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}