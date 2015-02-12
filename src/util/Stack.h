/*
 * Stack.h
 *
 *  Created on: 08/02/2015
 *      Author: cams7
 */

#ifndef STACK_H_
#define STACK_H_

#include "Node.h"

namespace SISBARC {
template<typename T>
class Stack {
private:
	Node<T>* _root;

public:
	Stack();
	virtual ~Stack();

	void push(T* const /*&*/); // push element
	void pop();          // pop element
	T* top() const;       // return top element
	bool empty() const;  // return true if empty.
};

} /* namespace SISBARC */

#include "Stack.cpp"
#endif /* STACK_H_ */

