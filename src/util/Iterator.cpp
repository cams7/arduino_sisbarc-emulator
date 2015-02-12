/*
 * Stack.cpp
 *
 *  Created on: 08/02/2015
 *      Author: cams7
 */

#ifdef ITERATOR_H_

#include "Iterator.h"

#include <stdlib.h>
//#include <stdio.h>

namespace SISBARC {

	template<typename T>
	Iterator<T>::Iterator() :
	_root(NULL),_current(NULL),_next(NULL),_hasNext(false),_nextChanged(false),size(0x0000) {
		//printf("New Iterator\n");
	}

	template<typename T>
	Iterator<T>::~Iterator() {
		if (!isEmpty()) {
			Iterator<T>* i = iterator();
			while (i->hasNext()) {
				i->remove();
			}
		}
		//printf("Delete Iterator\n");
	}

	template<typename T>
	Node<T>* Iterator<T>::getNext() /*const*/{
		if(!_nextChanged) {
			_current = _next;
			if(_current != NULL) {
				setNext(_current->getNext());
			}
		}
		return _current;
	}

	template<typename T>
	void Iterator<T>::setNext(Node<T>* const next) {
		this->_next = next;
		_hasNext = (next != NULL);
		_nextChanged =true;
	}

	template<typename T>
	void Iterator<T>::push(T* const element) {
		if (!isEmpty()) {
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
			setNext(_root);
		}
		size++;
	}

	template<typename T>
	void Iterator<T>::remove() {
		if (!isEmpty()) {
			Node<T>* node = getNext();

			bool isOnlyNode = true;

			if (node->getPrevious() != NULL) {
				node->getPrevious()->setNext(node->getNext());
				isOnlyNode = false;
			}

			if (node->getNext() != NULL) {
				node->getNext()->setPrevious(node->getPrevious());
				if(node->getPrevious() == NULL) {
					_root = node->getNext();
				}
				isOnlyNode = false;
			}

			delete node;

			if(isOnlyNode) {
				_root=NULL;
				setNext(NULL);
			}

			_current = NULL;
			size--;
		}
	}

	template<typename T>
	T* Iterator<T>::next() /*const*/{

		Node<T>* node = getNext();
		T* element;
		if(node!=NULL) {
			element = node->getElement();
		} else {
			element = NULL;
		}
		return element;
	}

	template<typename T>
	bool Iterator<T>::isEmpty() const {
		return (_root == NULL);
	}

	template<typename T>
	bool Iterator<T>::hasNext() /*const*/{
		_nextChanged = false;
		return _hasNext;
	}

	template<typename T>
	Iterator<T>* Iterator<T>::iterator() /*const*/{
		setNext(_root);
		return this;
	}

} /* namespace SISBARC */

#endif /* ITERATOR_H_ */
