/*
 * Stack.cpp
 *
 *  Created on: 08/02/2015
 *      Author: cams7
 */

#ifdef STACK_H_

#include "Stack.h"

#include <stdlib.h>
//#include <stdio.h>

namespace SISBARC {
	template<typename T>
	Stack<T>::Stack() :
	_root(NULL) {
		//printf("New Stack\n");
	}

	template<typename T>
	Stack<T>::~Stack() {
		while (!empty()) {
			pop();
		}
		//printf("Delete Stack\n");
	}

	template<typename T>
	void Stack<T>::push(T* const/*&*/element) {
		if (!empty()) {
			Node<T>* previous;
			Node<T>* next;

			previous = _root;
			next = _root->getNext();

			while (next != NULL) {
				previous = next;
				next = next->getNext();
			}

			next = new Node<T>(element);
			previous->setNext(next);
			next->setPrevious(previous);
		} else {
			_root=new Node<T>(element);
		}
	}

	template<typename T>
	void Stack<T>::pop() {
		if (!empty()) {
			Node<T>* previous;
			Node<T>* next;

			previous = _root;
			next = _root->getNext();

			while (next != NULL) {
				previous = next;
				next = next->getNext();
			}

			if (previous->getPrevious() != NULL) {
				previous->getPrevious()->setNext(NULL);
			} else if (previous->getNext() == NULL) {
				_root=NULL;
			}
			delete previous;
		}
	}

	template<typename T>
	T* Stack<T>::top() const {
		if (!empty()) {
			Node<T>* previous;
			Node<T>* next;

			previous = _root;
			next = _root->getNext();

			while (next != NULL) {
				previous = next;
				next = next->getNext();
			}

			return previous->getElement();
		}

		return NULL;
	}

	template<typename T>
	bool Stack<T>::empty() const {
		return (_root == NULL);
	}

} /* namespace SISBARC */

#endif /* STACK_H_ */
