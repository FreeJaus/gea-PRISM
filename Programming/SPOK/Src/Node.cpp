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

#include "../Headers/Node.h"

//Inival allows to reset to first letter of charset
Node::Node(char _value, char _inival, int _valuepos, bool _signaled, Node *_prev){
	this->value = _value;
	this->inival = _inival;
	this->valuepos = _valuepos;
	this->signaled = _signaled;
	this->prev = _prev;
}

void Node::Permute(char _value){
	this->value = _value;
	this->valuepos++;
}

void Node::Reset(){
	this->value = this->inival;
	this->valuepos = 0;
	this->signaled = true;
}