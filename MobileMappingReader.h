#pragma once
#include <string>

#include "PointCloud.h"

namespace EFTDEM {
	class MobileMappingReader {
	protected:
		static void printOutput(const PointCloud &pointCloud, int approximateNumLines);

	public:
		static void readPointsFromFile(PointCloud &pointCloud, const std::string &path, int debug);
	};
} // EFTDEM
