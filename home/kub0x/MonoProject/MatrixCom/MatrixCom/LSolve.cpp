#include <cmath>
#include <iostream>
#include "LSolve.hpp"

LEquation* LSolve::GetEquation(std::vector<int> a, int row, int col, int rowsize){
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

float* LSolve::CreateSystem(std::vector<int> a){
	int rowsize = sqrt(a.size()), r = -1;
	float *m = new float[rowsize*rowsize*rowsize*rowsize];
	for (int i = 0; i < rowsize; i++){
		for (int j = 0; j < rowsize; j++){
			r++;
			LEquation *leq = GetEquation(a, i, j, rowsize);
			leq->Simplify();
			leq->Sort();
			leq->Print();
			int *cm = leq->CoefMatrix();
			for (int k = 0; k < rowsize*rowsize; k++){
				m[r*rowsize*rowsize + k] = cm[k];
			}
			delete leq;
		}
	}
	return m;
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
	//for (int i=0; i < _size; i++)
	//	std::cout << _rhs[i] << std::endl;
	for (int i=0; i < size; i++)
		rhs[i] = _rhs[i];
	return v;
}