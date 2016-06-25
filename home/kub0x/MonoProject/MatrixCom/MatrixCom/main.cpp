#include <iostream>
#include <cstring>
#include <vector>
#include <limits>
#include <iomanip>
#include <lapacke/lapacke.h>

#include "LSolve.hpp"


void PrintMatrix(double *m, int r, int c){
	for (int i = 0; i < r; i++){
		for (int j = 0; j < c; j++){
			std::cout << std::setprecision (15) << m[i*c +j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void PrintFMatrix(float *m, int r, int c){
	for (int i = 0; i < r; i++){
		for (int j = 0; j < c; j++){
			std::cout << std::setprecision (20) << m[i*c +j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

int main (int argc, char *argv[])
{
	std::vector<int> v;
	v.push_back(541); 
	v.push_back(-317); 
	v.push_back(101);
	v.push_back(51);
	float* m = LSolve::GetInstance()->CreateSystem(v);
	lapack_int n=v.size(), nrhs = 1, lda = v.size(), ldb = 1, r = 0;
	PrintFMatrix(m, n, lda);
	float b[ldb*nrhs] = {0};
	lapack_int x[n] = {0};
	//for (int i = 0; i < ldb*nrhs; i++){ b[i]=0; s[i]=0; }
	//LAPACKE_sgelss(LAPACK_ROW_MAJOR, n, lda, nrhs, m, lda, b, ldb, s, 5.0f, &r);
	//std::cout <<  LAPACKE_dgetrf(LAPACK_ROW_MAJOR, n, n, m, lda, ipiv) << std::endl;
	//std::cout << LAPACKE_dgetri(LAPACK_ROW_MAJOR, n, m, lda, ipiv) << std::endl;
	//PrintMatrix(m, n, lda);
	//PrintMatrix(b, nrhs, ldb);

	std::cout << LAPACKE_sgelsy(LAPACK_ROW_MAJOR, n, lda, nrhs, m, lda, b, ldb, x, 0.007f, &r) << std::endl;
	std::cout << "Rank: " << r << std::endl;

	delete [] m;

	m = LSolve::GetInstance()->CreateSystem(v);
	//PrintMatrix(m, n, lda);
	std::vector<double> vars; vars.push_back(1); vars.push_back(2);

	std::vector<double*> undet = LSolve::GetInstance()->UnDetCoef(m, vars, r);

	delete [] m;

	double *um = undet.at(0);
	double *ub = undet.at(1);

	n = r, lda = r, ldb = 1;
	lapack_int ipiv[n];
	//PrintMatrix(um, n, lda);
	//PrintMatrix(ub, nrhs, ldb);
	std::cout << LAPACKE_dgesv(LAPACK_ROW_MAJOR, n, nrhs, um, lda, ipiv, ub, ldb) << std::endl;
	//PrintMatrix(um, n, lda);
	PrintMatrix(ub, r, nrhs);

	return 0;
}