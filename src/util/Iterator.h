/*
 * Stack.h
 *
 *  Created on: 08/02/2015
 *      Author: cams7
 */

#ifndef ITERATOR_H_
#define ITERATOR_H_

#include "Node.h"

namespace SISBARC {

template<typename T>
class Iterator {
private:
	Node<T>* _root;
	Node<T>* _current;
	Node<T>* _next;

	bool _hasNext;
	bool _nextChanged;

	Node<T>* getNext() /*const*/;
	void setNext(Node<T>* const);

protected:
	Iterator();
	virtual ~Iterator();

	uint16_t size;

	void push(T* const); // add element
public:
	bool isEmpty() const;  // return true if empty.

	bool hasNext() /*const*/;
	T* next() /*const*/;       // return next element
	void remove();          // remove element

	Iterator<T>* iterator() /*const*/;
};

} /* namespace SISBARC */

#include "Iterator.cpp"
#endif /* ITERATOR_H_ */

