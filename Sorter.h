#pragma once
#include "PointCloud.h"
#include "SYCLState.h"

namespace EFTDEM {
	class Sorter {
	public:
		static void sortPointCloud(PointCloud &pointCloud, SYCLState &syclState);
	};
} // EFTDEM
