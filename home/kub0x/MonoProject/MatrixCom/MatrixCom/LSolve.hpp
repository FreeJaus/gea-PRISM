#pragma once

#include <vector>
#include "Singleton.hpp"
#include "LEquation.hpp"

class LSolve : public Singleton<LSolve> {


	friend class Singleton<LSolve>;

	private:

		LSolve() {}

		LEquation* GetEquation(std::vector<int> a, int row, int col, int rowsize);
		
	public:

		float* CreateSystem(std::vector<int> a);
		std::vector<double*> UnDetCoef(float *s, std::vector<double> vars, int rank);

};