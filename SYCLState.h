
#pragma once
#include <sycl/sycl.hpp>

#include "Point.h"

namespace EFTDEM {
	class SYCLState {
	public:
		sycl::queue queue;
		std::optional<sycl::buffer<Point<double>>> pointsBuffer;
		std::optional<sycl::buffer<std::size_t>> gridCellIndicesBuffer;
		std::optional<sycl::buffer<float, 2>> heightsBuffer;
	};
}
