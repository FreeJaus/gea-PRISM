#pragma once

#include <vector>
#include "Unknown.hpp"

class LEquation {

	private:

		int len;
		std::vector<Unknown*> uk;
		int Simplify(int var);

	public:

		LEquation(std::vector<Unknown*> _uk, int _len) { uk = _uk; len = _len; }

		static bool Comparer(Unknown* a, Unknown* b) { return a->GetPos() < b->GetPos(); }

		void Print();
		void Simplify();
		void Sort();
		int* CoefMatrix();
};
