#pragma once
#include "PointCloud.h"
#include "SYCLState.h"

namespace EFTDEM {
	class Rasterizer {
	protected:
		static void printOutput(const PointCloud &pointCloud, SYCLState &syclState, int approximateNumLines);

	public:
		static void rasterizePointCloud(PointCloud &pointCloud, SYCLState &syclState, int debug);
	};
} // EFTDEM
