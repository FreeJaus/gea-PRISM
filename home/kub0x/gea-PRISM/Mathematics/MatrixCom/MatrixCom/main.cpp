#include <iostream>
#include <cmath>
#include <vector>

#include "LSolve.hpp"

int main (int argc, char *argv[])
{
	std::vector<double> v;
	v.push_back(52); 
	v.push_back(7); 
	v.push_back(1);
	v.push_back(-3);
	v.push_back(12); 
	v.push_back(71); 
	v.push_back(10);
	v.push_back(31);
	v.push_back(-13);
	v.push_back(21);
	v.push_back(-34);
	v.push_back(17); 
	v.push_back(43); 
	v.push_back(-10);
	v.push_back(9);
	v.push_back(7);

	int n = std::sqrt(v.size());
	std::vector<double> vars; vars.push_back(7); vars.push_back(21); vars.push_back(13); vars.push_back(74);
	double *rhs = LSolve::GetInstance()->CreateSystem(v, vars);
	LSolve::GetInstance()->MulMatrix(n, v, rhs, vars, n, n);

	return 0;
}