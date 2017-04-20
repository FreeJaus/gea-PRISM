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
