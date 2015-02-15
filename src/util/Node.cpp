/*
 * Node.cpp
 *
 *  Created on: 08/02/2015
 *      Author: cams7
 */
#ifdef NODE_H_

#include "Node.h"

#include <stdlib.h>
//#include <stdio.h>

namespace SISBARC {

	template<typename T>
	Node<T>::Node(T* const element) :
	_element(element), _next(NULL), _previous(NULL) {
		//printf("New Node\n");
	}

	template<typename T>
	Node<T>::~Node() {
		delete _element;
		//printf("Delete Node\n");
	}

	template<typename T>
	T* Node<T>::getElement() const {
		return _element;
	}

	template<typename T>
	Node<T>* Node<T>::getNext() const {
		return _next;
	}

	template<typename T>
	void Node<T>::setNext(Node<T>* const next) {
		this->_next = next;
	}

	template<typename T>
	Node<T>* Node<T>::getPrevious() const {
		return _previous;
	}

	template<typename T>
	void Node<T>::setPrevious(Node<T>* const previous) {
		this->_previous = previous;
	}

} /* namespace SISBARC */

#endif /* NODE_H_ */
