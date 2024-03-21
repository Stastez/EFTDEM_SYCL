
#pragma once

#include <stdexcept>

#include "Point.h"

template<typename T>
T &EFTDEM::Point<T>::operator[](const int index) {
	switch (index) {
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		default:
			throw std::out_of_range{"Index " + std::to_string(index) + " is out of bounds!"};
	}
}
