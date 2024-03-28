
#pragma once
#include "PointCloud.h"
#include "SYCLState.h"

namespace EFTDEM {
	class RadialFiller {
	protected:
		static void printOutput(const PointCloud &pointCloud, SYCLState &syclState, int approximateNumLines);

	public:
		static void fill(const PointCloud &pointCloud, SYCLState &syclState, unsigned numSteps, int debug);
	};
} // EFTDEM
