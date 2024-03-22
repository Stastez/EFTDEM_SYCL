
#pragma once
#include <sycl/sycl.hpp>

#include "Point.h"

namespace EFTDEM {
	class SYCLState {
	public:
		sycl::queue queue;
		sycl::buffer<Point<double> > pointsBuffer;
		sycl::buffer<std::size_t> gridCellIndicesBuffer;
		sycl::buffer<float> heightsBuffer;
	};
}
