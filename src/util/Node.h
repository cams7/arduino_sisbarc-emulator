/*
 * Node.h
 *
 *  Created on: 08/02/2015
 *      Author: cams7
 */

#ifndef NODE_H_
#define NODE_H_

namespace SISBARC {

template<typename T>
class Node {
private:
	T* _element;

	Node<T>* _next;
	Node<T>* _previous;

public:
	Node(T* const);
	virtual ~Node();

	T* getElement() const;

	Node<T>* getNext() const;
	void setNext(Node<T>* const);

	Node<T>* getPrevious() const;
	void setPrevious(Node<T>* const);
};

} /* namespace SISBARC */

#include "Node.cpp"
#endif /* NODE_H_ */
