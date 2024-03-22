#pragma once
#include "PointCloud.h"
#include "SYCLState.h"

namespace EFTDEM {
	class Sorter {
	protected:
		static void printOutput(const PointCloud &pointCloud, SYCLState &syclState, int approximateNumLines);

	public:
		static void sortPointCloud(PointCloud &pointCloud, SYCLState &syclState, int debug);
	};
} // EFTDEM
