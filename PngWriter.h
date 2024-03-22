
#pragma once
#include "PointCloud.h"

namespace EFTDEM {
	class PngWriter {
	public:
		static void exportPointCloud(const PointCloud &pointCloud, const std::string &path);
	};
} // EFTDEM
