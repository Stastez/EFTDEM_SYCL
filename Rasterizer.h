#pragma once
#include "PointCloud.h"
#include "SYCLState.h"

namespace EFTDEM {
	class Rasterizer {
	public:
		static void rasterizePointCloud(PointCloud &pointCloud, SYCLState syclState);
	};
} // EFTDEM
