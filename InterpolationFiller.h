
#pragma once
#include "PointCloud.h"
#include "SYCLState.h"

namespace EFTDEM {

class InterpolationFiller {
	protected:
		static void printOutput(const PointCloud &pointCloud, SYCLState &syclState, int approximateNumLines);

	public:
		static void fill(const PointCloud &pointCloud, SYCLState &syclState, unsigned kernelRadius, int debug);
};

} // EFTDEM
