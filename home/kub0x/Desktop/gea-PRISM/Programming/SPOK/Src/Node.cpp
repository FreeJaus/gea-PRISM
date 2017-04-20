#include "../Headers/Node.h"

Node::Node(char _value, int _valuepos, bool _signaled, Node *_prev){
	this->value = _value;
	this->inival = _value;
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