/*
 * List.h
 *
 *  Created on: 08/02/2015
 *      Author: cams7
 */

#ifndef LIST_H_
#define LIST_H_

#include <inttypes.h>

#include "Node.h"
#include "Iterator.h"

namespace SISBARC {

template<typename T>
class List: public Iterator<T> {
public:
	List();
	virtual ~List();

	T* get(uint16_t const&) /*const*/;

	void add(T* const); // push element
	T* remove(uint16_t const&) /*const*/;

	uint16_t size() const;
};

} /* namespace SISBARC */

#include "List.cpp"
#endif /* LIST_H_ */
