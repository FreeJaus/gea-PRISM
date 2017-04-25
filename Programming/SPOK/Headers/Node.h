/*
Copyright 2017 Borja Gomez - kub0x@elhacker.net

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

class Node final{

private:

	char value, inival;
	int valuepos;
	bool signaled;
	Node* prev;

public:

	Node(char _value, Node* _prev);
	Node(char _value, int _valuepos, bool _signaled, Node* _prev);

	void Permute(char _value);
	void Reset();
	char getValue() { return this->value; }
	int getValuePos() { return this->valuepos; }
	bool IsSignaled() { return this->signaled; }
	Node* Prev(){ return this->prev; }

};
