
#pragma once
#include <sycl/sycl.hpp>

#include "PointCloud.h"

namespace EFTDEM {

class Sorter {
public:
	static void sortPointCloud(PointCloud& pointCloud, sycl::queue &queue);
};

} // EFTDEM
