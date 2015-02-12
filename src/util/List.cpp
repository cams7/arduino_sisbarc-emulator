/*
 * List.cpp
 *
 *  Created on: 08/02/2015
 *      Author: cams7
 */
#ifdef LIST_H_

#include "List.h"

#include <stdlib.h>
//#include <stdio.h>

namespace SISBARC {
	template<typename T>
	List<T>::List():
	Iterator<T>() {
		//printf("New List\n");
	}

	template<typename T>
	List<T>::~List() {
		//printf("Delete List\n");
	}

	template<typename T>
	void List<T>::add(T* const element) {
		Iterator<T>::push(element);
	}

	template<typename T>
	uint16_t List<T>::size() const {
		return Iterator<T>::size;
	}

	template<typename T>
	T* List<T>::get(uint16_t const& index) /*const*/{
		T* element = NULL;

		if(!Iterator<T>::isEmpty()) {
			uint16_t count = 0x0000;
			Iterator<T>* i = Iterator<T>::iterator();
			T* next;
			while (i->hasNext()) {
				next = i->next();
				if(count==index) {
					element = next;
					break;
				}
				count++;
			}
		}

		return element;
	}

	template<typename T>
	T* List<T>::remove(uint16_t const& index) /*const*/{
		T* element = NULL;

		if(!Iterator<T>::isEmpty()) {
			uint16_t count = 0x0000;
			Iterator<T>* i = Iterator<T>::iterator();
			T* next;
			while (i->hasNext()) {
				next = i->next();
				if(count==index) {
					element = next;
					i->remove();
					break;
				}
				count++;
			}
		}
		return element;
	}

} /* namespace SISBARC */
#endif /* LIST_H_ */
