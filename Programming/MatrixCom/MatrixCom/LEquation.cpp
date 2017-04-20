#include <iostream>
#include <algorithm>
#include "LEquation.hpp"

int* LEquation::CoefMatrix(){ //Assumed array's ordered
	int *m = new int[len*len];
	int i = 0;
	for (std::vector<Unknown*>::iterator it = uk.begin(); it != uk.end(); it++){
		m[i] = (*it)->GetCoef();
		i++;
	}
	return m;
}

void LEquation::Sort(){
	std::sort(uk.begin(), uk.end(), &LEquation::Comparer);
	std::vector<Unknown*> neweq;
	for (int i = 0; i < len*len; i++){
		bool found = false;
		for (std::vector<Unknown*>::iterator it = uk.begin(); it != uk.end(); it++){
			if ((*it)->GetPos() == i){
				neweq.push_back(*it);
				found=true;
				break;
			}
		}
		if (!found)
		neweq.push_back(new Unknown(0,i));
	}
	uk = neweq;
}

int LEquation::Simplify(int var){
	int coef = 0, offset = 0;
	std::vector<Unknown*> cpy(uk);
	for (int i = 0; i < cpy.size(); i++){
		if (var == cpy.at(i)->GetPos()){
			coef += cpy.at(i)->GetCoef();
			uk.erase(uk.begin() + i - offset);
			offset++;
		}
	}
	return coef;
}

void LEquation::Simplify(){
	std::vector<Unknown*> neweq;
	int i = 0;
	for (std::vector<Unknown*>::iterator it = uk.begin(); it != uk.end(); it = uk.begin()){
		int var = (*it)->GetPos();
		int coef = Simplify(var);
		//if (coef != 0)
			neweq.push_back(new Unknown(coef,var));
		i++;
	}
	uk = neweq;
}
void LEquation::Print(){
	for (std::vector<Unknown*>::iterator it = uk.begin(); it != uk.end(); it++){
		std::cout << (*it)->GetCoef() << (*it)->GetPos() << " ";
	}
	std::cout << std::endl;
}