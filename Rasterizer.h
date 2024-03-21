#pragma once
#include <sycl/sycl.hpp>

#include "PointCloud.h"

namespace EFTDEM {
	class Rasterizer {
	public:
		static void rasterizePointCloud(PointCloud &pointCloud, sycl::queue &queue);
	};
} // EFTDEM
