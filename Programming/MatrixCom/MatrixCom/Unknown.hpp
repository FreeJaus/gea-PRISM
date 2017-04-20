#pragma once

class Unknown {

	private:

		int coef, pos;

	public:

		Unknown(int _coef, int _pos) { coef = _coef; pos = _pos; }
		~Unknown() {}

		int GetPos() { return pos; }
		int GetCoef() { return coef; }
};