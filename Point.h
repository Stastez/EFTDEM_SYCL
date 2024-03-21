
#pragma once

namespace EFTDEM {

template <typename T>
class Point {
public:
	T x, y, z;
	T& operator[](int index);
};

} // EFTDEM

#include "Point.inl"
